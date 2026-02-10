#include "FileFormat_Jpeg.h"

#include <Types/Graphics\Image8.h>

FileFormat_Jpeg::FileFormat_Jpeg()
{
	Name = L"JPEG";

	Extensions.Add(L"jpg");
	Extensions.Add(L"jpeg");
	Extensions.Add(L"jpe");
}

Type FileFormat_Jpeg::GetObjectType() const
{
	return TYPEOF(Image);
}

Object* FileFormat_Jpeg::LoadFile(const String& filename, void* /*params*/)
{
	jpeg_decompress_struct cinfo;
	jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	FILE* file;

	if ((_wfopen_s(&file, filename, L"rb")) != 0)
	{
		jpeg_destroy_decompress(&cinfo);
		return null;
	}

	jpeg_stdio_src(&cinfo, file);

	jpeg_read_header(&cinfo, true);

	jpeg_start_decompress(&cinfo);

	cinfo.dither_mode = JDITHER_NONE;
	cinfo.two_pass_quantize = false;
	cinfo.colormap = NULL;

	int imageWidth = (int)cinfo.output_width;
	int imageHeight = (int)cinfo.output_height;

	double imageAspect = 1.0;

	if (cinfo.density_unit == 0)
	{
		imageAspect = (double)cinfo.X_density / (double)cinfo.Y_density;
	}

	Image8* image8 = new Image8(imageWidth, imageHeight); //, imageAspect);

	byte *buffer = (byte*)malloc((size_t)(cinfo.output_width * cinfo.output_components * cinfo.rec_outbuf_height));

	while (cinfo.output_scanline < cinfo.output_height)
	{
		int filePos = cinfo.output_width * cinfo.output_scanline;

		jpeg_read_scanlines(&cinfo, (JSAMPARRAY)&buffer, cinfo.rec_outbuf_height);

		int* b = (int*)image8->Buffer;
		int iWidth = 0;

		if (cinfo.output_components == 3)
		{
			iWidth = image8->Width * cinfo.rec_outbuf_height;

			int i3;

			for (int i = 0; i < iWidth; i++)
			{
				i3 = i * 3;

				b[filePos + i] = 
					((buffer[i3    ] & 0xFF) << 16) |
					((buffer[i3 + 1] & 0xFF) <<  8) |
					( buffer[i3 + 2] & 0xFF)        |
					0xFF000000;
			}
		}
		else if (cinfo.output_components == 1)
		{
			iWidth = image8->Width;

			for (int i = 0; i < iWidth; i++)
			{
				b[filePos + i] = 
					((buffer[i] & 0xFF) << 16) |
					((buffer[i] & 0xFF) <<  8) |
					 (buffer[i] & 0xFF)        |
					0xFF000000;
			}
		}
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	free(buffer);
	fclose(file);

	return image8;
}

bool FileFormat_Jpeg::SaveFile(Object* obj, const String& filename, void* params)
{
	assert(obj);
	assert(typeof(obj) == TYPEOF(Image8));

	if (typeof(obj) != GetObjectType())
		return false;

	Image8* image8 = (Image8*)obj;

	Params* jsp = (Params*)params;
	
	int quality = 100;

	if (jsp)
		quality = jsp->quality;

	jpeg_compress_struct cinfo;
	jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	FILE* file;

	if ((_wfopen_s(&file, filename, L"wb")) != 0)
	{
		jpeg_destroy_compress(&cinfo);
		return false;
	}

	jpeg_stdio_dest(&cinfo, file);

	cinfo.image_width		= image8->Width;
	cinfo.image_height		= image8->Height;
	cinfo.input_components	= 3;
	cinfo.in_color_space	= JCS_RGB;

	jpeg_set_defaults(&cinfo);

	jpeg_set_quality(&cinfo, quality, TRUE);

	jpeg_start_compress(&cinfo, TRUE);

	int* buffer = (int*)image8->Buffer;

	byte* scanline = (byte*)malloc(image8->Width * 3 * sizeof(char));

	for (int y = 0; y < image8->Height; y++)
	{
		int _width = image8->Width;

		for (int x = 0; x < _width; x++)
		{
			scanline[x * 3 + 2] = (char)( buffer[x]        & 0xFF);
			scanline[x * 3 + 1] = (char)((buffer[x] >>  8) & 0xFF);
			scanline[x * 3    ] = (char)((buffer[x] >> 16) & 0xFF);
		}

		jpeg_write_scanlines(&cinfo, (JSAMPARRAY)&scanline, 1);

		buffer += image8->Width;
	}

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);

	free(scanline);
	fclose(file);

	return true;
}