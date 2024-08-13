#ifndef _VIRTIO_H
#define _VIRTIO_H

#include "types.h"

#define VIRTIO_CONSOLE_F_SIZE (0)
#define VIRTIO_CONSOLE_F_MULTIPORT (1)
#define VIRTIO_CONSOLE_F_EMERG_WRITE (2)

struct Virtio;
typedef struct Virtio *VirtioDevice;

VirtioDevice virtio(u64 base, u64 desired_features);
void virtio_clear_interrupt(VirtioDevice device);

void virtio_console_emerge_write(VirtioDevice console_base, u32 c);

///* write `n` elements from `buffer` */
//void virtio_console_write(VirtioDevice console, u8 *buffer, u16 n);
//
///* read at most `size` elements to `buffer` and return number of read bytes */
//u16 virtio_console_read(VirtioDevice console, u8 **buffer, u16 size);

#endif
