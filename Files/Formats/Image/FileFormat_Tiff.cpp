#include "FileFormat_Tiff.h"

#include <Types/Graphics\Image8.h>

FileFormat_Tiff::FileFormat_Tiff()
{
	Name = L"TIFF";

	Extensions.Add(L"tiff");
	Extensions.Add(L"tif");
}

Type FileFormat_Tiff::GetObjectType() const
{
	return TYPEOF(Image);
}

Object* FileFormat_Tiff::LoadFile(const String& filename, void* /* params */)
{
	char* szFilename = (char*)calloc(((String&)filename).Length + 1, sizeof(char));
	size_t charsConverted;
	wcstombs_s(&charsConverted, szFilename, ((String&)filename).Length + 1, filename, ((String&)filename).Length);

	TIFFSetWarningHandler(NULL);

	TIFF* tif = TIFFOpen(szFilename, "r");

	if (tif)
	{
		uint32 imageWidth;
		uint32 imageHeight;
		uint16 samplesPerPixel;
		uint16 bitsPerSample;

		uint16 config;
		tdata_t buf;
		uint32 row;

		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &imageWidth);
		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &imageHeight);
		TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &config);
		TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesPerPixel);
		TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitsPerSample);

		Image8* image8 = new Image8(imageWidth, imageHeight); //1.0);

		buf = _TIFFmalloc(TIFFScanlineSize(tif));
		
		unsigned int lineSamples = imageWidth * samplesPerPixel;

		switch (config)
		{
		case PLANARCONFIG_CONTIG:
			{
				for (row = 0; row < imageHeight; row++)
				{
					TIFFReadScanline(tif, buf, row);

					int* imgBuf = (int*)image8->Buffer + imageWidth * row;

					switch (samplesPerPixel)
					{
					case 1: // bitmap
						{
							switch (bitsPerSample)
							{
							case 1:
								//char* bitBuf = (char*)buf;

								break;
							}

							break;
						}
					case 3: // RGB
						{
							switch (bitsPerSample)
							{
							case 8:
								{
									char* charBuf = (char*)buf;

									for (unsigned int i = 0; i < lineSamples; i += samplesPerPixel)
									{
										int r = (int)charBuf[i]		& 0xFF;
										int g = (int)charBuf[i + 1] & 0xFF;
										int b = (int)charBuf[i + 2] & 0xFF;

										imgBuf[i / samplesPerPixel] = 
											  0xFF000000 
											| (r << 16) 
											| (g << 8) 
											|  b;
									}

									break;
								}
							case 16:
								{
									short* shortBuf = (short*)buf;

									for (unsigned int i = 0; i < lineSamples; i += samplesPerPixel)
									{
										int r = ((int)shortBuf[i]     & 0xFFFF) >> 8;
										int g = ((int)shortBuf[i + 1] & 0xFFFF) >> 8;
										int b = ((int)shortBuf[i + 2] & 0xFFFF) >> 8;

										imgBuf[i / samplesPerPixel] = 
											  0xFF000000 
											| (r << 16) 
											| (g << 8) 
											|  b;
									}

									break;
								}
							}

							break;
						}
					case 4: // RGBA
						{
							switch (bitsPerSample)
							{
							case 8:
								{
									int* intBuf = (int*)buf;

									for (int i = 0; i < (int)imageWidth; i++)
									{
										int col = intBuf[i];
										imgBuf[i] = 
											   (col & 0xFF00FF00) 
											| ((col & 0x000000FF) << 16) 
											| ((col & 0x00FF0000) >> 16);
									}

									break;
								}
							case 16:
								{
									short* shortBuf = (short*)buf;

									for (unsigned int i = 0; i < lineSamples; i += samplesPerPixel)
									{
										int r = ((int)shortBuf[i]     & 0xFFFF) >> 8;
										int g = ((int)shortBuf[i + 1] & 0xFFFF) >> 8;
										int b = ((int)shortBuf[i + 2] & 0xFFFF) >> 8;
										int a = ((int)shortBuf[i + 3] & 0xFFFF) >> 8;

										imgBuf[i / samplesPerPixel] = 
											  (a << 24) 
											| (r << 16) 
											| (g << 8) 
											|  b;
									}

									break;
								}
							}

							break;
						}
					}
				}

				break;
			}
		case PLANARCONFIG_SEPARATE:
			{
				//uint16 s, nsamples;

				//TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &nsamples);
				//
				//for (s = 0; s < nsamples; s++)
				//{
				//	for (row = 0; row < imageHeight; row++)
				//	{
				//		TIFFReadScanline(tif, buf, row, s);
				//	}
				//}

				break;
			}
		}

		_TIFFfree(buf);
		TIFFClose(tif);

		return image8;
	}

	return null;
}

bool FileFormat_Tiff::SaveFile(Object* obj, const String& /* fileName */, void* /* params */)
{
	assert(obj);

	return false;
}