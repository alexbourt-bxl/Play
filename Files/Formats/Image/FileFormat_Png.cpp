#include "FileFormat_Png.h"

#include <Types/Graphics\Image8.h>

FileFormat_Png::FileFormat_Png()
{
	Name = L"PNG";
	Extensions.Add(L"png");
}

Type FileFormat_Png::GetObjectType() const
{
	return TYPEOF(Image);
}

Object* FileFormat_Png::LoadFile(const String& filename, void* /* params */)
{
	FILE* file = null;

	if ((_wfopen_s(&file, filename, L"rb")) != 0)
		return null;

	char header[8];

	fread(header, 1, sizeof(header), file);

	if (png_sig_cmp((png_bytep)header, 0, sizeof(header)))
	{
		fclose(file);
		return null;
	}

	png_struct* png = png_create_read_struct(
		PNG_LIBPNG_VER_STRING, 
		null, 
		null, 
		null);

	if (!png)
	{
		fclose(file);
		return null;
	}

	png_info* info = png_create_info_struct(png);

	if (!info)
	{
		png_destroy_read_struct(
			&png, 
			(png_info**)null, 
			(png_info**)null);

		fclose(file);
		return null;
	}

	png_info* end = png_create_info_struct(png);

	if (!end)
	{
		png_destroy_read_struct(
			&png, 
			&info, 
			(png_info**)null);

		fclose(file);
		return null;
	}

	png_init_io(png, file);
	png_set_sig_bytes(png, sizeof(header));

	int transforms = 
		PNG_TRANSFORM_STRIP_16	|
		PNG_TRANSFORM_PACKING	|
		PNG_TRANSFORM_EXPAND	|
		PNG_TRANSFORM_SHIFT		|
		PNG_TRANSFORM_GRAY_TO_RGB;

	png_read_png(png, info, transforms, null);

	Image8* image8 = new Image8(info->width, info->height); //, 1.0);

	int* imgBuffer = (int*)image8->Buffer;

	byte** rows = png_get_rows(png, info);

	switch (info->color_type)
	{
	case PNG_COLOR_TYPE_PALETTE:
		{
			bool hasTrans = png_get_valid(png, info, PNG_INFO_tRNS) != 0;

			int yWidth;
			png_color* pc;
			int t;

			int br = info->background.red;
			int bg = info->background.green;
			int bb = info->background.blue;

			for (uint y = 0; y < info->height; y++)
			{
				yWidth = y * info->width;
				
				for (uint x = 0; x < info->width; x++)
				{
					pc = &info->palette[rows[y][x]];

					int r = (int)pc->red;
					int g = (int)pc->green;
					int b = (int)pc->blue;
					int a = 0xFF;

					if (hasTrans)
					{
						a = (int)info->trans_alpha[rows[y][x]];

						r = br + INT_MULT(r - br, a, t);
						g = bg + INT_MULT(g - bg, a, t);
						b = bb + INT_MULT(b - bb, a, t);
					}

					imgBuffer[yWidth + x] = 
						  (a << 24)
						| (r << 16) 
						| (g << 8) 
						|  b;
				}
			}

			break;
		}
	//case PNG_COLOR_TYPE_GRAY:
	//	{
	//		int yWidth;

	//		for (uint y = 0; y < info->height; y++)
	//		{
	//			yWidth = y * info->width;
	//			
	//			for (uint x = 0; x < info->width; x++)
	//			{
	//				int v = (int)rows[y][x * info->channels] & 0xFF;
	//				imgBuffer[yWidth + x] = 
	//					  0xFF000000 
	//					| (v << 16) 
	//					| (v << 8) 
	//					|  v;
	//			}
	//		}

	//		break;
	//	}
	//case PNG_COLOR_TYPE_GA:
	//	{
	//		int yWidth;

	//		int bv = info->background.gray;

	//		for (uint y = 0; y < info->height; y++)
	//		{
	//			yWidth = y * info->width;
	//			
	//			for (uint x = 0; x < info->width; x++)
	//			{
	//				int v = (int)rows[y][x * info->channels    ] & 0xFF;
	//				int a = (int)rows[y][x * info->channels + 1] & 0xFF;

	//				v = bv + (((v - bv) * a) >> 8);

	//				imgBuffer[yWidth + x] = 
	//					  (a << 24) 
	//					| (v << 16) 
	//					| (v << 8) 
	//					|  v;
	//			}
	//		}

	//		break;
	//	}
	case PNG_COLOR_TYPE_RGB:
		{
			int yWidth;

			for (uint y = 0; y < info->height; y++)
			{
				yWidth = y * info->width;
				
				for (uint x = 0; x < info->width; x++)
				{
					int r = (int)rows[y][x * info->channels    ] & 0xFF;
					int g = (int)rows[y][x * info->channels + 1] & 0xFF;
					int b = (int)rows[y][x * info->channels + 2] & 0xFF;

					imgBuffer[yWidth + x] = 
						  0xFF000000 
						| (r << 16) 
						| (g << 8) 
						|  b;
				}
			}

			break;
		}
	case PNG_COLOR_TYPE_RGBA:
		{
			int yWidth;
			int t;

			int br = info->background.red;
			int bg = info->background.green;
			int bb = info->background.blue;

			for (uint y = 0; y < info->height; y++)
			{
				yWidth = y * info->width;
				
				for (uint x = 0; x < info->width; x++)
				{
					int r = (int)rows[y][x * info->channels    ] & 0xFF;
					int g = (int)rows[y][x * info->channels + 1] & 0xFF;
					int b = (int)rows[y][x * info->channels + 2] & 0xFF;
					int a = (int)rows[y][x * info->channels + 3] & 0xFF;

					r = br + INT_MULT(r - br, a, t);
					g = bg + INT_MULT(g - bg, a, t);
					b = bb + INT_MULT(b - bb, a, t);

					imgBuffer[yWidth + x] = 
						  (a << 24) 
						| (r << 16) 
						| (g << 8) 
						|  b;

					Color8::Premult(imgBuffer[yWidth + x]);
				}
			}

			break;
		}
	}

	png_destroy_read_struct(&png, &info, &end);
	fclose(file);

	return image8;
}

bool FileFormat_Png::SaveFile(Object* obj, const String& filename, void* /*params*/)
{
	assert(obj);
	assert(typeof(obj) == TYPEOF(Image8));

	Image8* image8 = (Image8*)obj;

	FILE* file = null;

	if ((_wfopen_s(&file, filename, L"wb")) != 0)
		return false;

	png_struct* png = png_create_write_struct(
		PNG_LIBPNG_VER_STRING, 
		null, 
		null, 
		null);

	if (!png)
	{
		fclose(file);
		return false;
	}

	png_info* info = png_create_info_struct(png);

	if (!info)
	{
		png_destroy_write_struct(
			&png, 
			(png_info**)null);

		fclose(file);
		return false;
	}

	png_init_io(png, file);

	png_set_IHDR(
		png,
		info,
		image8->Width,
		image8->Height,
		8,
		PNG_COLOR_TYPE_RGB_ALPHA,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);

	png_write_info(png, info);

	png_set_bgr(png);

	byte** rows = (byte**)malloc(image8->Height * sizeof(byte*));

	for (int i = 0; i < image8->Height; i++)
		rows[i] = image8->Buffer + i * image8->Width * 4;

	png_write_rows(
		png,
		rows,
		image8->Height);

	free(rows);

	png_write_end(png, info);

	png_destroy_write_struct(&png, &info);
	fclose(file);

	return true;
}