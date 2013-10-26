/*
 * Copyright (c) 2013, Mathias Hällman. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "graphics/TextureLoader.h"

#include <memory>

extern "C" {
#include <png.h>
}

#include "graphics/TextureResource.h"

static File* s_file;

static void ReadPNG(png_structp pngPtr, png_bytep data, png_size_t length)
{
	s_file->Read(data, length);
}

std::shared_ptr<Resource> TextureLoader::Load(
	const ResourceKey& key,
	const FileManager& fileManager) const
{
	File file;
	s_file = &file;
	fileManager.Open(key.GetFilePath(), file);

	// Read header to see if it's a PNG file.
	const int headerByteSize = 8;
	png_byte header[headerByteSize];
	file.Read(header, headerByteSize);
	if (png_sig_cmp(header, 0, headerByteSize) != 0)
	{
		return nullptr;
	}

	// Setup state structs.
	png_structp pngPtr = png_create_read_struct(
		PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop infoPtr = png_create_info_struct(pngPtr);
	png_infop endInfo = png_create_info_struct(pngPtr);

	// Setup reading function.
	png_set_read_fn(pngPtr, NULL, ReadPNG);
	png_set_sig_bytes(pngPtr, headerByteSize);

	// Get info about the PNG.
	png_read_info(pngPtr, infoPtr);

	int bitDepth, colorType;
	png_uint_32 width = 0, height = 0;

	png_get_IHDR(pngPtr, infoPtr, &width, &height, &bitDepth, &colorType,
		NULL, NULL, NULL);

	png_read_update_info(pngPtr, infoPtr);

	int rowByteSize = png_get_rowbytes(pngPtr, infoPtr);

	// Pixel data for OpenGL.
	auto pixels = std::unique_ptr<png_byte[]>(
		new png_byte[rowByteSize * height]);

	// Row pixel data for reading.
	auto rowPtrs = std::shared_ptr<png_bytep>(
		new png_bytep[height],
		std::default_delete<png_bytep>());

	for (png_uint_32 row = 0; row < height; ++row)
	{
		png_bytep rowStart = pixels.get() + row * rowByteSize;
		rowPtrs.get()[height - 1 - row] = rowStart;
	}

	// Read the pixel data.
	png_read_image(pngPtr, rowPtrs.get());

	// Cleanup.
	png_destroy_read_struct(&pngPtr, &infoPtr, &endInfo);

	auto renderResourceHandle = g_renderer->GenerateTexture(width, height,
		std::unique_ptr<GLvoid>(static_cast<GLvoid*>(pixels.release())));

	return std::make_shared<TextureResource>(renderResourceHandle);
}
