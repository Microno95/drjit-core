/*
    enoki-jit/texture.h -- creating and querying of 1D/2D/3D textures on CUDA

    Copyright (c) 2021 Wenzel Jakob <wenzel.jakob@epfl.ch>

    All rights reserved. Use of this source code is governed by a BSD-style
    license that can be found in the LICENSE file.
*/

#include "jit.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * \brief Allocate CUDA texture memory
 *
 * Allocates memory for a texture of size \c ndim with a total of
 * <tt>shape[0] x ... x shape[ndim - 1]</tt> texels/voxels, where each
 * voxel is furthermore composed of \c n_channels color components.
 * The value of the \c n_channels argument must be greater or equal than 1.
 * The function returns an opaque texture handle.
 *
 * The \c filter_mode parameter supports the following options:
 *
 * <ul>
 * <li><tt>filter_mode == 0</tt>: Nearest-neighbor sampling</li>
 * <li><tt>filter_mode == 1</tt>: Linear/bilinear/trilinear interpolation</li>
 * <ul>
 *
 * The \c wrap_mode parameter supports the following options:
 *
 * <ul>
 * <li><tt>wrap_mode == 0</tt>: Repeat</li>
 * <li><tt>wrap_mode == 1</tt>: Clamp</li>
 * <li><tt>wrap_mode == 2</tt>: Mirror</li>
 * <ul>
 *
 * Further modes (e.g. MIP-mapping) may be added in the future.
 */
extern JIT_EXPORT void *jit_cuda_tex_create(size_t ndim, const size_t *shape,
                                            size_t n_channels,
                                            int filter_mode JIT_DEF(1),
                                            int wrap_mode JIT_DEF(0));

/**
 * \brief Copy from device to texture memory
 *
 * Fills the texture with data from device memory at \c src_ptr. The other
 * arguments are analogous to \ref jit_cuda_tex_create(). The operation runs
 * asynchronously.
 */
extern JIT_EXPORT void jit_cuda_tex_memcpy_d2t(size_t ndim, const size_t *shape,
                                               const void *src_ptr,
                                               void *dst_texture_handle);

/**
 * \brief Copy from texture to device memory
 *
 * Implements the reverse of \ref jit_cuda_tex_memcpy_d2t
 */
extern JIT_EXPORT void jit_cuda_tex_memcpy_t2d(size_t ndim, const size_t *shape,
                                               const void *src_texture_handle,
                                               void *dst_ptr);

/**
 * \brief Performs a CUDA texture lookup
 *
 * \param ndim
 *     Dimensionality of the texture
 *
 * \param handle
 *     Texture handle (returned value of \ref jit_cuda_tex_create())
 *
 * \param pos
 *     Pointer to a list of <tt>ndim - 1 </tt> float32 variable indices
 *     encoding the position of the texture lookup
 *
 * \param out
 *     Pointer to an array of size equal to the number of channels in the
 *     texture, which will receive the lookup result.
 */
extern JIT_EXPORT void jit_cuda_tex_lookup(size_t ndim,
                                           const void *texture_handle,
                                           const uint32_t *pos, uint32_t mask,
                                           uint32_t *out);

/// Destroys the provided texture handle
extern JIT_EXPORT void jit_cuda_tex_destroy(void *texture_handle);

#if defined(__cplusplus)
}
#endif
