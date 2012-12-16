/*********************************************************************NVMH4****
Path:  NVSDK\Common\src\dds_common
File:  dds_common.cpp

Copyright NVIDIA Corporation 2002
TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED
*AS IS* AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS
OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL NVIDIA OR ITS SUPPLIERS
BE LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES
WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS)
ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.



Comments:

dds loader.



******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
//
// Description:
// 
// Loads DDS images (DXTC1, DXTC3, DXTC5, RGB (888, 888X), and RGBA (8888) are
// supported) for use in OpenGL. Image is flipped when its loaded as DX images
// are stored with different coordinate system. If file has mipmaps and/or 
// cubemaps then these are loaded as well. Volume textures can be loaded as 
// well but they must be uncompressed.
//
// When multiple textures are loaded (i.e a volume or cubemap texture), 
// additional faces can be accessed using the array operator. 
//
// The mipmaps for each face are also stored in a list and can be accessed like 
// so: image.get_mipmap() (which accesses the first mipmap of the first 
// image). To get the number of mipmaps call the get_num_mipmaps function for
// a given texture.
//
// Call the is_volume() or is_cubemap() function to check that a loaded image
// is a volume or cubemap texture respectively. If a volume texture is loaded
// then the get_depth() function should return a number greater than 1. 
// Mipmapped volume textures and DXTC compressed volume textures are supported.
//
///////////////////////////////////////////////////////////////////////////////
//
// Update: 23/10/2002
//
// CMAUGHAN
// A branch from dds to dds_common to make a common file that doens't have the
// API specific stuff in, so that DX8/9 can use this file's useful capability of
// loading dds files into memory.

#ifndef __DDS_COMMON_H__
#define __DDS_COMMON_H__

#ifdef WIN32
#  include <windows.h>
#endif

#include <string>
#include <vector>
#include <assert.h>

namespace dds_common
{
	enum
	{
		FORMAT_DXT1,
		FORMAT_DXT2,
		FORMAT_DXT3,
		FORMAT_DXT4,
		FORMAT_DXT5,
		FORMAT_RGBA,
		FORMAT_RGB,
		FORMAT_L8
	};

	const unsigned long DDS_FOURCC = 0x00000004;
	const unsigned long DDS_RGB	= 0x00000040;
	const unsigned long DDS_RGBA   = 0x00000041;
	const unsigned long DDS_DEPTH  = 0x00800000;

	const unsigned long DDS_COMPLEX = 0x00000008;
	const unsigned long DDS_CUBEMAP = 0x00000200;
	const unsigned long DDS_VOLUME  = 0x00200000;

	// Same enums as d3d types.h
	const unsigned long D3DFMT_R8G8B8			   = 20;
	const unsigned long D3DFMT_A8R8G8B8			 = 21;
	const unsigned long D3DFMT_X8R8G8B8			 = 22;
	const unsigned long D3DFMT_R5G6B5			   = 23;
	const unsigned long D3DFMT_X1R5G5B5			 = 24;
	const unsigned long D3DFMT_A1R5G5B5			 = 25;
	const unsigned long D3DFMT_A4R4G4B4			 = 26;
	const unsigned long D3DFMT_R3G3B2			   = 27;
	const unsigned long D3DFMT_A8				   = 28;
	const unsigned long D3DFMT_A8R3G3B2			 = 29;
	const unsigned long D3DFMT_X4R4G4B4			 = 30;
	const unsigned long D3DFMT_A2B10G10R10		  = 31;
	const unsigned long D3DFMT_A8B8G8R8			 = 32;
	const unsigned long D3DFMT_X8B8G8R8			 = 33;
	const unsigned long D3DFMT_G16R16			   = 34;
	const unsigned long D3DFMT_A2R10G10B10		  = 35;
	const unsigned long D3DFMT_A16B16G16R16		 = 36;
	const unsigned long D3DFMT_A8P8				 = 40;
	const unsigned long D3DFMT_P8				   = 41;
	const unsigned long D3DFMT_L8				   = 50;
	const unsigned long D3DFMT_A8L8				 = 51;
	const unsigned long D3DFMT_A4L4				 = 52;
	const unsigned long D3DFMT_V8U8				 = 60;
	const unsigned long D3DFMT_L6V5U5			   = 61;
	const unsigned long D3DFMT_X8L8V8U8			 = 62;
	const unsigned long D3DFMT_Q8W8V8U8			 = 63;
	const unsigned long D3DFMT_V16U16			   = 64;
	const unsigned long D3DFMT_A2W10V10U10		  = 67;
	const unsigned long D3DFMT_D16_LOCKABLE		 = 70;
	const unsigned long D3DFMT_D32				  = 71;
	const unsigned long D3DFMT_D15S1				= 73;
	const unsigned long D3DFMT_D24S8				= 75;
	const unsigned long D3DFMT_D24X8				= 77;
	const unsigned long D3DFMT_D24X4S4			  = 79;
	const unsigned long D3DFMT_D16				  = 80;
	const unsigned long D3DFMT_D32F_LOCKABLE		= 82;
	const unsigned long D3DFMT_D24FS8			   = 83;
	const unsigned long D3DFMT_L16				  = 81;
	const unsigned long D3DFMT_Q16W16V16U16		 =110;

	const unsigned long FOURCC_DXT1 = 0x31545844; //(MAKEFOURCC('D','X','T','1'))
	const unsigned long FOURCC_DXT3 = 0x33545844; //(MAKEFOURCC('D','X','T','3'))
	const unsigned long FOURCC_DXT5 = 0x35545844; //(MAKEFOURCC('D','X','T','5'))

	// s10e5 formats (16-bits per channel)
	const unsigned long D3DFMT_R16F				 = 111;
	const unsigned long D3DFMT_G16R16F			  = 112;
	const unsigned long D3DFMT_A16B16G16R16F		= 113;

	// IEEE s23e8 formats (32-bits per channel)
	const unsigned long D3DFMT_R32F				 = 114;
	const unsigned long D3DFMT_G32R32F			  = 115;
	const unsigned long D3DFMT_A32B32G32R32F		= 116;
	const unsigned long D3DFMT_CxV8U8			   = 117;

	struct DDS_PIXELFORMAT
	{
		unsigned long dwSize;
		unsigned long dwFlags;
		unsigned long dwFourCC;
		unsigned long dwRGBBitCount;
		unsigned long dwRBitMask;
		unsigned long dwGBitMask;
		unsigned long dwBBitMask;
		unsigned long dwABitMask;
	};

	struct DXTColBlock
	{
		unsigned short col0;
		unsigned short col1;

		unsigned char row[4];
	};

	struct DXT3AlphaBlock
	{
		unsigned short row[4];
	};

	struct DXT5AlphaBlock
	{
		unsigned char alpha0;
		unsigned char alpha1;
		
		unsigned char row[6];
	};

	struct DDS_HEADER
	{
		unsigned long dwSize;
		unsigned long dwFlags;
		unsigned long dwHeight;
		unsigned long dwWidth;
		unsigned long dwPitchOrLinearSize;
		unsigned long dwDepth;
		unsigned long dwMipMapCount;
		unsigned long dwReserved1[11];
		DDS_PIXELFORMAT ddspf;
		unsigned long dwCaps1;
		unsigned long dwCaps2;
		unsigned long dwReserved2[3];
	};

	class CSurface
	{
		friend class CTexture;
		friend class CDDSImage;	

		public:
			CSurface();
			CSurface(int w, int h, int d, int imgsize);
			CSurface(const CSurface &copy);
			CSurface &operator= (const CSurface &rhs);
			virtual ~CSurface();

			operator char*();

			void create(int w, int h, int d, int imgsize);
			void clear();

			inline int get_width() { return width; }
			inline int get_height() { return height; }
			inline int get_depth() { return depth; }
			inline int get_size() { return size; }

		protected:
			int width;
			int height;
			int depth;
			int size;

			char *pixels;	   
	};

	class CTexture : public CSurface
	{
		friend class CDDSImage;

		public:
			CTexture();
			CTexture(int w, int h, int d, int imgSize);
			CTexture(const CTexture &copy);
			CTexture &operator= (const CTexture &rhs);
			~CTexture();

			inline CSurface &get_mipmap(int index) 
			{ 
				assert(index < (int)mipmaps.size());
				return mipmaps[index]; 
			}

			inline int get_num_mipmaps() { return (int)mipmaps.size(); }
		protected:
			std::vector<CSurface> mipmaps;
	};

	class CDDSImage
	{
		public:
			CDDSImage();
			~CDDSImage();

			bool load(std::string filename, bool flipImage = true);
			void clear();
			
			operator char*();
			CTexture &operator[](int index);

			inline unsigned int get_num_images()
			{
				assert(valid);
				return (int)images.size();
			}
			inline int get_width() 
			{
				assert(valid);
				assert(images.size() > 0);
				
				return images[0].get_width(); 
			}

			inline int get_height()
			{
				assert(valid);
				assert(images.size() > 0);
				
				return images[0].get_height(); 
			}

			inline int get_depth()
			{
				assert(valid);
				assert(images.size() > 0);
				
				return images[0].get_depth(); 
			}

			inline int get_size()
			{
				assert(valid);
				assert(images.size() > 0);

				return images[0].get_size();
			}

			inline int get_num_mipmaps() 
			{ 
				assert(valid);
				assert(images.size() > 0);

				return images[0].get_num_mipmaps(); 
			}

			inline CSurface &get_mipmap(int index) 
			{ 
				assert(valid);
				assert(images.size() > 0);
				assert(index < images[0].get_num_mipmaps());

				return images[0].get_mipmap(index); 
			}

			inline int get_components() { return components; }
			inline int get_format() { return format; }
			inline unsigned long get_d3dformat() { return d3dformat; }

			inline bool is_compressed() { return compressed; }
			inline bool is_cubemap() { return cubemap; }
			inline bool is_volume() { return volume; }
			inline bool is_valid() { return valid; }
			int size_dxtc(int width, int height);
			int size_rgb(int width, int height);			
		private:
			int clamp_size(int size);
			int get_line_width(int width, int bpp);
			
			void align_memory(CTexture *surface);

			void flip(char *image, int width, int height, int depth, int size);

			void swap(void *byte1, void *byte2, int size);
			void flip_blocks_dxtc1(DXTColBlock *line, int numBlocks);
			void flip_blocks_dxtc3(DXTColBlock *line, int numBlocks);
			void flip_blocks_dxtc5(DXTColBlock *line, int numBlocks);
			void flip_dxt5_alpha(DXT5AlphaBlock *block);

			int format;
			unsigned long d3dformat;
			int components;
			bool compressed;
			bool cubemap;
			bool volume;
			bool valid;

			std::vector<CTexture> images;

	};
}
#endif
