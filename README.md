# Dump Physical Address of A Process

*Wang Xiaoqiang<wang_xiaoq@126.com> Distributed & Embedded System Lab [DSLab](http://dslab.lzu.edu.cn/)*

The module is used to dump all the physical addresses(page frames) used by a process. It taskes a command line parameter to specified which process to dump.

## Usage

        1. make
        2. sudo insmod dump-phys.ko task_name="your_process_name"
        3. dmesg
        4. make uninstall

## Example

For the step 2. we can give a example:

`sudo insmod dump-phys.ko task_name="init"`

The command above dumps the physical addresses of process "init". Then use:

`dmesg`

Part of the output is as follows:

*[72240.260786] linear address: 0xb7552000, physical address: not alloc<br/>
[72240.260790] linear address: 0xb7553000, physical address: 0x7e7ce000<br/>
[72240.260794] linear address: 0xb7554000, physical address: 0x7e7cf000<br/>
[72240.260797] linear address: 0xb7555000, physical address: not alloc<br/>
[72240.260801] linear address: 0xb7556000, physical address: not alloc<br/>
[72240.260805] linear address: 0xb7557000, physical address: not alloc<br/>
[72240.260808] linear address: 0xb7558000, physical address: not alloc<br/>
[72240.260812] linear address: 0xb7559000, physical address: not alloc<br/>
[72240.260816] linear address: 0xb755a000, physical address: not alloc<br/>
[72240.260820] linear address: 0xb755b000, physical address: 0x7dc96000<br/>
[72240.260823] linear address: 0xb755c000, physical address: not alloc<br/>
[72240.260827] linear address: 0xb755d000, physical address: not alloc<br/>
[72240.260831] linear address: 0xb755e000, physical address: 0x7e409000<br/>
[72240.260835] linear address: 0xb755f000, physical address: 0x7e40a000<br/>*

Each item in the output represents a pair of page and page frame addresses.

## LICENSE
This program is under the license of GPL v2.