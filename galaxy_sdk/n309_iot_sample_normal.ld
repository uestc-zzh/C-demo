/*
 * Copyright (c) 2019 Nuclei Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

 /* linker script of normal firmware */

OUTPUT_ARCH( "riscv" )

ENTRY( _start )

MEMORY
{
  flash : ORIGIN = 0x10010000, LENGTH = 1024K
  sdram : ORIGIN = 0x20200000, LENGTH = 128K /* Non-retention */
  siram : ORIGIN = 0x20220000, LENGTH = 64K  /* Retention */
  ipc   : ORIGIN = 0x20260000, LENGTH = 8K
  noncache : ORIGIN = 0x20262000, LENGTH = 56K
  ilm : ORIGIN = 0x80000000, LENGTH = 64K
  dlm : ORIGIN = 0x90000000, LENGTH = 64K
}
__FLASH_MEM_END = ORIGIN(flash) + LENGTH(flash);
__SDRAM_MEM_END = ORIGIN(sdram) + LENGTH(sdram);
__SIRAM_MEM_END = ORIGIN(siram) + LENGTH(siram);
__NONCACHE_MEM_END = ORIGIN(noncache) + LENGTH(noncache);
__ILM_MEM_END = ORIGIN(ilm) + LENGTH(ilm);
__DLM_MEM_END = ORIGIN(dlm) + LENGTH(dlm);

__ILM_BL_RSV = 4K;

PROVIDE(__STACK_SIZE = 2K);
PROVIDE(__HEAP_SIZE = 20K);
__TOT_STACK_SIZE = __STACK_SIZE;

__FLASH_PAGE_SIZE = 256;
__XIP_BASE_ADDR = 0x10000000;
__FW_FLASH_BASE = ORIGIN(flash) - __XIP_BASE_ADDR;
__FW_XIP_BASE = ORIGIN(flash);

__IPC_MEM_SIZE = LENGTH(ipc);

SECTIONS
{
    /******************************ilm********************************************/
    __ILM_VMA_BASE = ORIGIN(ilm);
    __ILM_LMA_BASE = ORIGIN(flash) + __FLASH_PAGE_SIZE;
    .ilm __ILM_VMA_BASE :
    {
        PROVIDE(__fw_entry = .);

        *(.vtable)
        KEEP (*(SORT_NONE(.init)))
        . = ALIGN(4);

        *(.text.unlikely .text.unlikely.*)
        *(.text.startup .text.startup.*)
        . = ALIGN(4);

        *port.*.o(.text*)
        *port.o(.text*)
        *portasm.*.o(.text*)
        *portasm.o(.text*)
        *clock*.o(.text*)
        *hal_flash*.o(.text*)
        *hal_spi*.o(.text*)
        *libos*.a:*(.text*)
        *os/FreeRTOS/Kernel/tasks.o(.text*)
        *os/FreeRTOS/Kernel/queue.o(.text*)
        *os/FreeRTOS/Kernel/list.o(.text*)
        *os/FreeRTOS/Kernel/timers.o(.text*)
        *os/FreeRTOS/Kernel/stream_buffer.o(.text*)
        *os/FreeRTOS/Kernel/croutine.o(.text*)
        *os/FreeRTOS/Kernel/event_groups.o(.text*)
        *os/FreeRTOS/Kernel/portable/Common/mpu_wrappers.o(.text*)
        *os/FreeRTOS/Kernel/portable/MemMang/heap_4.o(.text*)
        *libg_nano*.a:*(.text*)
        *libnosys*.a:*(.text*)
        *osal_task*.o(.text*)
        *osal_event*.o(.text*)
        *osal_heap*.o(.text*)
        *osal_lock*.o(.text*)
        *osal_notify*.o(.text*)
        *osal_time*.o(.text*)
        *osal_semaphore*.o(.text*)
        *osal_sys_state*.o(.text*)
        *heap_4_noncache*.o(.text*)
        *soc_sysctl*.o(.text*)
        *soc_pinmux*.o(.text*)
        *system_pegasus*.o(.text*)
        *vs_spiflash*.o(.text*)
        *clk*.o(.text*)
        *vs_iot_fix*.o(.text*)
        *vs_iot_pll*.o(.text*)
        *notifier*.o(.text*)
        *vs_qspi*.o(.text*)
        *vs_common*.o(.text*)
        *lib_a-memset*.o(.text*)
        *iomem*.o(.text*)
        *soc_irq*.o(.text*)
        *(.itext)
        . = ALIGN(64);
    }
    __ILM_SECTION_SIZE = SIZEOF(.ilm);
    __ILM_SECTION_PAGES = __ILM_SECTION_SIZE / __FLASH_PAGE_SIZE;
    __ILM_SECTION_PAGES += (((__ILM_SECTION_SIZE % __FLASH_PAGE_SIZE) > 0 ) ? 1 : 0);
    __ILM_SECTION_PAGES += 1;
    __ILM_VMA_END = __ILM_VMA_BASE + __FLASH_PAGE_SIZE * __ILM_SECTION_PAGES;
    __ILM_LMA_END = __ILM_LMA_BASE + __FLASH_PAGE_SIZE * __ILM_SECTION_PAGES;

    ASSERT((__ILM_SECTION_SIZE + __ILM_BL_RSV) <= LENGTH(ilm), "ilm space error!")
    /****************************************************************************/

    /************************************siram**********************************/
    __SIRAM_VMA_BASE = ORIGIN(siram);
    __SIRAM_LMA_BASE = __ILM_LMA_END + __FLASH_PAGE_SIZE;
    .siram __SIRAM_VMA_BASE :
    {
        *(.nonxip_text)
        . = ALIGN(64);
    }
    __SIRAM_SECTION_SIZE = SIZEOF(.siram);
    __SIRAM_SECTION_PAGES = __SIRAM_SECTION_SIZE / __FLASH_PAGE_SIZE;
    __SIRAM_SECTION_PAGES += (((__SIRAM_SECTION_SIZE % __FLASH_PAGE_SIZE) > 0 ) ? 1 : 0);
    __SIRAM_SECTION_PAGES += 1;
    __SIRAM_VMA_END = __SIRAM_VMA_BASE + __SIRAM_SECTION_PAGES * __FLASH_PAGE_SIZE;
    __SIRAM_LMA_END = __SIRAM_LMA_BASE +  __SIRAM_SECTION_PAGES * __FLASH_PAGE_SIZE;

    ASSERT((__SIRAM_SECTION_SIZE) <= LENGTH(siram), "siram space error!")
    /**************************************************************************/

    /***********************************sdram**********************************/
    __SDRAM_VMA_BASE = ORIGIN(sdram);
    __SDRAM_LMA_BASE = __SIRAM_LMA_END + __FLASH_PAGE_SIZE;
    .sdram __SDRAM_VMA_BASE : ALIGN(8)
    {
        KEEP(*(.data.ctest*))
        *(.data .data.*)
        *(.sdram_data)
        *(.sys_inherited_data)
        *(.gnu.linkonce.d.*)

        . = ALIGN(8);
        *port.*.o(.rodata .rodata.*)
        *port.o(.rodata .rodata.*)
        *portasm.*.o(.rodata .rodata.*)
        *portasm.o(.rodata .rodata.*)
        *clock*.o(.rodata .rodata.*)
        *hal_flash*.o(.rodata .rodata.*)
        *hal_spi*.o(.rodata .rodata.*)
        *libos*.a:*(.rodata .rodata.*)
        *os/FreeRTOS/Kernel/tasks.o(.rodata .rodata.*)
        *os/FreeRTOS/Kernel/queue.o(.rodata .rodata.*)
        *os/FreeRTOS/Kernel/list.o(.rodata .rodata.*)
        *os/FreeRTOS/Kernel/timers.o(.rodata .rodata.*)
        *os/FreeRTOS/Kernel/stream_buffer.o(.rodata .rodata.*)
        *os/FreeRTOS/Kernel/croutine.o(.rodata .rodata.*)
        *os/FreeRTOS/Kernel/event_groups.o(.rodata .rodata.*)
        *os/FreeRTOS/Kernel/portable/Common/mpu_wrappers.o(.rodata .rodata.*)
        *os/FreeRTOS/Kernel/portable/MemMang/heap_4.o(.rodata .rodata.*)
        *osal_task*.o(.rodata .rodata.*)
        *osal_event*.o(.rodata .rodata.*)
        *osal_heap*.o(.rodata .rodata.*)
        *osal_lock*.o(.rodata .rodata.*)
        *osal_notify*.o(.rodata .rodata.*)
        *osal_time*.o(.rodata .rodata.*)
        *osal_semaphore*.o(.rodata .rodata.*)
        *osal_sys_state*.o(.rodata .rodata.*)
        *heap_4_noncache*.o(.rodata .rodata.*)
        *soc_sysctl*.o(.rodata .rodata.*)
        *soc_pinmux*.o(.rodata .rodata.*)
        *system_pegasus*.o(.rodata .rodata.*)
        *vs_spiflash*.o(.rodata .rodata.*)
        *clk*.o(.rodata .rodata.*)
        *vs_iot_fix*.o(.rodata .rodata.*)
        *vs_iot_pll*.o(.rodata .rodata.*)
        *notifier*.o(.rodata .rodata.*)
        *vs_qspi*.o(.rodata .rodata.*)
        *vs_common*.o(.rodata .rodata.*)
        *lib_a-memset*.o(.rodata .rodata.*)
        *iomem*.o(.rodata .rodata.*)
        *soc_irq*.o(.rodata .rodata.*)

        . = ALIGN(4);
        _net_buf_pool_list = .;
        _net_buf_pool_list_start = .;
        *(._net_buf_pool.*);
        _net_buf_pool_list_end = .;

        . = ALIGN(4);
        _k_queue_list_start  = .;
        *(._k_queue.static.*);
        _k_queue_list_end  = .;

        . = ALIGN(4);
        _k_mem_slab_list_start  = .;
        KEEP(*(._k_mem_slab.static.*));
        _k_mem_slab_list_end  = .;

        . = ALIGN(4);
        _bt_gatt_service_static_list_start  = .;
        KEEP(*(._bt_gatt_service_static.static.*));
        _bt_gatt_service_static_list_end  = .;

        . = ALIGN(4);
        _bt_l2cap_fixed_chan_list_start  = .;
        KEEP(*(._bt_l2cap_fixed_chan.static.*));
        _bt_l2cap_fixed_chan_list_end  = .;

        . = ALIGN(4);
        _settings_handler_static_list_start  = .;
        KEEP(*(._settings_handler_static.static.*));
        _settings_handler_static_list_end  = .;

        . = ALIGN(4);
        _k_sem_list_start  = .;
        KEEP(*(._k_sem.static.*));
        _k_sem_list_end  = .;

        . = ALIGN(4);
        _bt_conn_cb_list_start  = .;
        KEEP(*(._bt_conn_cb.static.*));
        _bt_conn_cb_list_end  = .;

       . = ALIGN(8);
       PROVIDE( __global_pointer$ = . + 0x800 );
       *(.sdata .sdata.* .sdata*)
       *(.gnu.linkonce.s.*)
       . = ALIGN(8);

       PROVIDE( __tls_base = . );
       *(.tdata .tdata.* .gnu.linkonce.td.*)
       . = ALIGN(64);
    }
    __SDRAM_SECTION_SIZE = SIZEOF(.sdram);
    __SDRAM_SECTION_PAGES = __SDRAM_SECTION_SIZE / __FLASH_PAGE_SIZE;
    __SDRAM_SECTION_PAGES += (((__SDRAM_SECTION_SIZE % __FLASH_PAGE_SIZE) > 0 ) ? 1 : 0);
    __SDRAM_SECTION_PAGES += 1;
    __SDRAM_VMA_END = __SDRAM_VMA_BASE + __SDRAM_SECTION_SIZE;
    __SDRAM_LMA_END = __SDRAM_LMA_BASE + __SDRAM_SECTION_PAGES * __FLASH_PAGE_SIZE;

    PROVIDE( _data_lma = LOADADDR(.sdram) );
    PROVIDE( _data = ADDR(.sdram) );
    PROVIDE( _edata = . );
    /*****************************************************************************/

    /*********************************flash***************************************/
    __FLASH_VMA_BASE = __SDRAM_LMA_END + __FLASH_PAGE_SIZE;
    __FLASH_LMA_BASE = __SDRAM_LMA_END + __FLASH_PAGE_SIZE;
    .flash __FLASH_VMA_BASE :
    {
        . = ALIGN(4);
        *libble*.a:*(.text*)
        . = ALIGN(4);
        __init_start = .;
        __init_PRE_KERNEL_1_start = .;
        KEEP(*(SORT(.z_init_PRE_KERNEL_1[0-9]_*)));
        KEEP(*(SORT(.z_init_PRE_KERNEL_1[1-9][0-9]_*)));
        __init_PRE_KERNEL_2_start = .;
        KEEP(*(SORT(.z_init_PRE_KERNEL_2[0-9]_*)));
        KEEP(*(SORT(.z_init_PRE_KERNEL_2[1-9][0-9]_*)));
        __init_POST_KERNEL_start = .;
        KEEP(*(SORT(.z_init_POST_KERNEL[0-9]_*)));
        KEEP(*(SORT(.z_init_POST_KERNEL[1-9][0-9]_*)));
        __init_APPLICATION_start = .;
        KEEP(*(SORT(.z_init_APPLICATION[0-9]_*)));
        KEEP(*(SORT(.z_init_APPLICATION[1-9][0-9]_*)));
        __init_end = .;

        . = ALIGN(4);
        *(.stext)
        . = ALIGN(4);
        *(.text*)

        . = ALIGN(8);
        *(.srodata.cst*)
        *(.srodata .srodata.*)
        *(.rdata)
        *(.rodata .rodata.*)
        *(.gnu.linkonce.r.*)
        . = ALIGN(64 * 1024);
    }
    __FLASH_SECTION_SIZE = SIZEOF(.flash);
    __FLASH_SECTION_PAGES = __FLASH_SECTION_SIZE / __FLASH_PAGE_SIZE;
    __FLASH_SECTION_PAGES += (((__FLASH_SECTION_SIZE % __FLASH_PAGE_SIZE) > 0 ) ? 1 : 0);
    __FLASH_SECTION_PAGES += 1;
    __FLASH_VMA_END = __FLASH_VMA_BASE + __FLASH_SECTION_PAGES * __FLASH_PAGE_SIZE;
    __FLASH_LMA_END = __FLASH_LMA_BASE + __FLASH_SECTION_PAGES * __FLASH_PAGE_SIZE;

    PROVIDE( _text_lma = LOADADDR(.flash) );
    PROVIDE( _text = ADDR(.flash) );
    PROVIDE (_etext = .);
    /***************************************************************************/

    PROVIDE(__zsp_addr = __FLASH_LMA_END - __XIP_BASE_ADDR);

    /*****************************noinit and bss*******************************************/
    __NOINIT_VMA_BASE = __SDRAM_VMA_END;
    .noinit __NOINIT_VMA_BASE (NOLOAD): ALIGN(8)
    {
        *(.noinit.*)
        *(.sensor_data)
        *(.rsv_data)
        . = ALIGN(4);
    }
    __NOINIT_SECTION_SIZE = SIZEOF(.noinit);
    __NOINIT_VMA_END = __NOINIT_VMA_BASE + __NOINIT_SECTION_SIZE;

    __BSS_VMA_BASE = __NOINIT_VMA_END;
    PROVIDE( _fbss = __BSS_VMA_BASE);
    PROVIDE( __bss_start = __BSS_VMA_BASE);
    .bss __BSS_VMA_BASE (NOLOAD): ALIGN(4)
    {
       *(.sbss*)
       *(.gnu.linkonce.sb.*)
       *(.bss .bss.*)
       *(.gnu.linkonce.b.*)
       *(COMMON)
       . = ALIGN(4);
    }
    __BSS_SECTION_SIZE = SIZEOF(.bss);
    __BSS_VMA_END = __BSS_VMA_BASE + __BSS_SECTION_SIZE;
    PROVIDE(_end = .);

    ASSERT((__BSS_VMA_END - __SDRAM_VMA_BASE) <= (LENGTH(sdram)), "sdram space error!")
    /****************************************************************************/

    /**************************heap and stack***********************/
    __NON_CACHEABLE_VMA_BASE = ORIGIN(noncache);
    .noncacheable __NON_CACHEABLE_VMA_BASE (NOLOAD) :
    {
        *(.noncache_data)
    }
    __NON_CACHEABLE_SIZE = SIZEOF(.noncacheable);
    __NON_CACHEABLE_VMA_END =  __NON_CACHEABLE_VMA_BASE + __NON_CACHEABLE_SIZE;

    __HEAP_VMA_BASE = __NONCACHE_MEM_END - (__HEAP_SIZE + __TOT_STACK_SIZE);
    .heap __HEAP_VMA_BASE (NOLOAD) : ALIGN(16)
    {
        . = ALIGN(16);
        PROVIDE( __heap_start = . );
        PROVIDE(end = .);
        . += __HEAP_SIZE;
        . = ALIGN(16);
        PROVIDE( __heap_limit = . );
    }
    __HEAP_SECTION_SIZE = SIZEOF(.heap);
    __HEAP_VMA_END = __HEAP_VMA_BASE + __HEAP_SECTION_SIZE;

    __STACK_VMA_BASE = __HEAP_VMA_END;
    .stack __STACK_VMA_BASE (NOLOAD) :
    {
        . = ALIGN(16);
        PROVIDE( _heap_end = . );
        PROVIDE( __heap_end = . );
        PROVIDE( __StackLimit = . );
        PROVIDE( __StackBottom = . );
        . += __TOT_STACK_SIZE;
        . = ALIGN(16);
        PROVIDE( __StackTop = (. - 16));
        PROVIDE( _sp = (. - 16));
    }
    __STACK_SECTION_SIZE = SIZEOF(.stack);
    __STACK_VMA_END = __STACK_VMA_BASE + __STACK_SECTION_SIZE;

    ASSERT((__STACK_VMA_END - __NON_CACHEABLE_VMA_BASE) <= LENGTH(noncache), "Non cacheable space error!")
    /****************************************************************************/

    /********************************os heap************************************/
    __OSHEAP_VMA_BASE = ORIGIN(dlm);
    .osheap __OSHEAP_VMA_BASE (NOLOAD) : ALIGN(8)
    {
        *(.RTOS_HEAP_SEC)
    }
    __OSHEAP_SECTION_SIZE = SIZEOF(.osheap);
    __OSHEAP_VMA_END = __OSHEAP_VMA_BASE + __OSHEAP_SECTION_SIZE;

    ASSERT((__OSHEAP_SECTION_SIZE) <= LENGTH(dlm), "dlm space error!")
    /**************************************************************************/

    /********************************ipc************************************/
    __IPC_VMA_BASE = ORIGIN(ipc);
    .ipc __IPC_VMA_BASE (NOLOAD) :
    {
        __ipc_mem_start = .;
    }
    /**************************************************************************/
}
