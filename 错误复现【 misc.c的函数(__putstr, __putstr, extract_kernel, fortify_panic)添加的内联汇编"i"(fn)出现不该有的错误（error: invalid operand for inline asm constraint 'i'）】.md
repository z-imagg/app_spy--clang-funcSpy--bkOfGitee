# misc.c的函数(__putstr, __putstr, extract_kernel, fortify_panic)添加的内联汇编"i"(fn)出现不该有的错误（error: invalid operand for inline asm constraint 'i'）



##  浮现错误步骤:
```shell
cd /bal/clang-add-funcIdAsm/SrcFileFuncIdGenService
source /app/miniconda3/bin/activate
nohup   python WebMain.py & 


source /bal/script_basic/install-cmd-wrap.sh

cd /bal/linux-stable/

#丢弃clang插件上次的修改
git checkout -- lib/zlib_inflate/inffast.c
git checkout -- arch/x86/boot/compressed/../../../../lib/zlib_inflate/inftrees.c
git checkout -- arch/x86/boot/compressed/misc.c

#重新运行报错的编译命令
clang -v  -Wp,-MD,arch/x86/boot/compressed/.misc.o.d -nostdinc -isystem /app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/lib/clang/15.0.0/include -I./arch/x86/include -I./arch/x86/include/generated  -I./include -I./arch/x86/include/uapi -I./arch/x86/include/generated/uapi -I./include/uapi -I./include/generated/uapi -include ./include/linux/kconfig.h -D__KERNEL__ -Qunused-arguments -m32 -D__KERNEL__ -O1 -fno-strict-aliasing -fPIE -DDISABLE_BRANCH_PROFILING -march=i386 -mno-mmx -mno-sse -ffreestanding -fno-stack-protector -Wno-address-of-packed-member -Wa,-mrelax-relocations=no    -DKBUILD_BASENAME='"misc"'  -DKBUILD_MODNAME='"misc"' -c -o arch/x86/boot/compressed/misc.o arch/x86/boot/compressed/misc.c

```

## 错误日志 
> ```tail -f /bal/g-0.log``` 输出日志如下:
```txt

INFO:2024-01-09 09:24:32.560032@/bal/bin/clang:67:<module>:日志文件/bal/g-0.log锁定成功,立即退出循环
INFO:2024-01-09 09:24:32.560070@/bal/bin/clang:97:<module>:收到命令及参数（即sys.argv即字符串sysArgvAsStr）:【/bal/bin/clang -v -Wp,-MD,arch/x86/boot/compressed/.misc.o.d -nostdinc -isystem /app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/lib/clang/15.0.0/include -I./arch/x86/include -I./arch/x86/include/generated -I./include -I./arch/x86/include/uapi -I./arch/x86/include/generated/uapi -I./include/uapi -I./include/generated/uapi -include ./include/linux/kconfig.h -D__KERNEL__ -Qunused-arguments -m32 -D__KERNEL__ -O1 -fno-strict-aliasing -fPIE -DDISABLE_BRANCH_PROFILING -march=i386 -mno-mmx -mno-sse -ffreestanding -fno-stack-protector -Wno-address-of-packed-member -Wa,-mrelax-relocations=no -DKBUILD_BASENAME="misc" -DKBUILD_MODNAME="misc" -c -o arch/x86/boot/compressed/misc.o arch/x86/boot/compressed/misc.c】
INFO:2024-01-09 09:24:32.613448@/bal/cmd-wrap/interceptor_util.py:24:execute_script_file:执行脚本文件及结果： 脚本文件:【/bal/cmd-wrap/env-diff-show.sh】, retCode【0】,std_out【/tmp/env-diff--20240109092432_1704763472_562493550.txt
】,err_out【】
INFO:2024-01-09 09:24:32.670365@/bal/cmd-wrap/lark_parser/api_lark_parse_single_cmd.py:29:larkGetSrcFileFromSingleGccCmd:lark即将解析文本singleGccCmd：【/bal/bin/clang -v -Wp,-MD,arch/x86/boot/compressed/.misc.o.d -nostdinc -isystem /app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/lib/clang/15.0.0/include -I./arch/x86/include -I./arch/x86/include/generated -I./include -I./arch/x86/include/uapi -I./arch/x86/include/generated/uapi -I./include/uapi -I./include/generated/uapi -include ./include/linux/kconfig.h -D__KERNEL__ -Qunused-arguments -m32 -D__KERNEL__ -O1 -fno-strict-aliasing -fPIE -DDISABLE_BRANCH_PROFILING -march=i386 -mno-mmx -mno-sse -ffreestanding -fno-stack-protector -Wno-address-of-packed-member -Wa,-mrelax-relocations=no -DKBUILD_BASENAME="misc" -DKBUILD_MODNAME="misc" -c -o arch/x86/boot/compressed/misc.o arch/x86/boot/compressed/misc.c】
INFO:2024-01-09 09:24:32.752405@/bal/cmd-wrap/lark_parser/api_lark_parse_single_cmd.py:41:larkGetSrcFileFromSingleGccCmd:命令中的源文件相关字段为: -m32 -march=i386   -D__KERNEL__ -D__KERNEL__ -DDISABLE_BRANCH_PROFILING -DKBUILD_BASENAME="misc" -DKBUILD_MODNAME="misc" -Wp,-MD,arch/x86/boot/compressed/.misc.o.d -Wno-address-of-packed-member -Wa,-mrelax-relocations=no  -fno-strict-aliasing -fPIE -ffreestanding -fno-stack-protector   -isystem /app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/lib/clang/15.0.0/include  -I./arch/x86/include -I./arch/x86/include/generated -I./include -I./arch/x86/include/uapi -I./arch/x86/include/generated/uapi -I./include/uapi -I./include/generated/uapi     -include ./include/linux/kconfig.h -c arch/x86/boot/compressed/misc.c
INFO:2024-01-09 09:24:32.752460@/bal/cmd-wrap/interceptor_util.py:35:execute_cmd:真实命令（字符串_cmdReceived）:【/app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/bin/clang -Xclang -load -Xclang /bal/clang-add-funcIdAsm/build/lib/libCTk.so -Xclang -add-plugin -Xclang CTk -fsyntax-only -v -Wp,-MD,arch/x86/boot/compressed/.misc.o.d -nostdinc -isystem /app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/lib/clang/15.0.0/include -I./arch/x86/include -I./arch/x86/include/generated -I./include -I./arch/x86/include/uapi -I./arch/x86/include/generated/uapi -I./include/uapi -I./include/generated/uapi -include ./include/linux/kconfig.h -D__KERNEL__ -Qunused-arguments -m32 -D__KERNEL__ -O1 -fno-strict-aliasing -fPIE -DDISABLE_BRANCH_PROFILING -march=i386 -mno-mmx -mno-sse -ffreestanding -fno-stack-protector -Wno-address-of-packed-member -Wa,-mrelax-relocations=no -DKBUILD_BASENAME="misc" -DKBUILD_MODNAME="misc" -c -o arch/x86/boot/compressed/misc.o arch/x86/boot/compressed/misc.c】
INFO:2024-01-09 09:24:32.977384@/bal/cmd-wrap/interceptor_util.py:66:execute_cmd:真实命令退出码,正常退出码:【0】
INFO:2024-01-09 09:24:32.977458@/bal/cmd-wrap/interceptor_util.py:71:execute_cmd:真实命令标准输出【查看，文件路径:arch/x86/boot/compressed/misc.c,mainFileId:1,frontendOptions.ProgramAction:25，Ctx.TUKind:0
提示，开始处理编译单元,文件路径:arch/x86/boot/compressed/misc.c,CTkAstConsumer:0x5615fac42380,mainFileId:1
开发查问题日志funcIsStatic_funcIsInline:【arch/x86/boot/compressed/misc.c:85:1;funcQualifiedName】,funcIsStatic=true,funcIsInline=false
开发查问题日志funcIsStatic_funcIsInline:【arch/x86/boot/compressed/misc.c:98:1;funcQualifiedName】,funcIsStatic=true,funcIsInline=false
开发查问题日志funcIsStatic_funcIsInline:【arch/x86/boot/compressed/misc.c:108:1;funcQualifiedName】,funcIsStatic=false,funcIsInline=false
开发查问题日志funcIsStatic_funcIsInline:【arch/x86/boot/compressed/misc.c:157:1;funcQualifiedName】,funcIsStatic=false,funcIsInline=false
开发查问题日志funcIsStatic_funcIsInline:【arch/x86/boot/compressed/misc.c:174:1;funcQualifiedName】,funcIsStatic=true,funcIsInline=false
开发查问题日志funcIsStatic_funcIsInline:【arch/x86/boot/compressed/misc.c:186:1;funcQualifiedName】,funcIsStatic=true,funcIsInline=false
开发查问题日志funcIsStatic_funcIsInline:【arch/x86/boot/compressed/misc.c:280:1;funcQualifiedName】,funcIsStatic=true,funcIsInline=false
开发查问题日志funcIsStatic_funcIsInline:【arch/x86/boot/compressed/misc.c:354:1;funcQualifiedName】,funcIsStatic=false,funcIsInline=false
开发查问题日志funcIsStatic_funcIsInline:【arch/x86/boot/compressed/misc.c:437:1;funcQualifiedName】,funcIsStatic=false,funcIsInline=false
插入'#pragma 消息'到文件arch/x86/boot/compressed/misc.c,对mainFileId:1,结果:false
】
INFO:2024-01-09 09:24:32.977538@/bal/cmd-wrap/interceptor_util.py:74:execute_cmd:真实命令错误输出【clang version 15.0.0 (git@github.com:llvm/llvm-project.git 4ba6a9c9f65bbc8bd06e3652cb20fd4dfc846137)
Target: i386-unknown-linux-gnu
Thread model: posix
InstalledDir: /app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/bin
Found candidate GCC installation: /usr/lib/gcc-cross/i686-linux-gnu/11
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/11
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/12
Selected GCC installation: /usr/lib/gcc-cross/i686-linux-gnu/11
Candidate multilib: .;@m32
Candidate multilib: 64;@m64
Candidate multilib: x32;@mx32
Selected multilib: .;@m32
 (in-process)
 "/app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/bin/clang-15" -cc1 -triple i386-unknown-linux-gnu -fsyntax-only -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name misc.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -relaxed-aliasing -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -ffreestanding -target-cpu i386 -target-feature -mmx -target-feature -sse -mllvm -treat-scalable-fixed-error-as-warning -debugger-tuning=gdb -v -fcoverage-compilation-dir=/bal/linux-stable -nostdsysteminc -nobuiltininc -resource-dir /app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/lib/clang/15.0.0 -dependency-file arch/x86/boot/compressed/.misc.o.d -MT arch/x86/boot/compressed/misc.o -sys-header-deps -isystem /app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/lib/clang/15.0.0/include -include ./include/linux/kconfig.h -I ./arch/x86/include -I ./arch/x86/include/generated -I ./include -I ./arch/x86/include/uapi -I ./arch/x86/include/generated/uapi -I ./include/uapi -I ./include/generated/uapi -D __KERNEL__ -D __KERNEL__ -D DISABLE_BRANCH_PROFILING -D "KBUILD_BASENAME=\"misc\"" -D "KBUILD_MODNAME=\"misc\"" -O1 -Wno-address-of-packed-member -fdebug-compilation-dir=/bal/linux-stable -ferror-limit 19 -fgnuc-version=4.2.1 -load /bal/clang-add-funcIdAsm/build/lib/libCTk.so -add-plugin CTk -faddrsig -x c arch/x86/boot/compressed/misc.c
clang -cc1 version 15.0.0 based upon LLVM 15.0.0 default target x86_64-unknown-linux-gnu
#include "..." search starts here:
#include <...> search starts here:
 ./arch/x86/include
 ./arch/x86/include/generated
 ./include
 ./arch/x86/include/uapi
 ./arch/x86/include/generated/uapi
 ./include/uapi
 ./include/generated/uapi
 /app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/lib/clang/15.0.0/include
End of search list.
】
INFO:2024-01-09 09:24:32.977685@/bal/cmd-wrap/interceptor_util.py:35:execute_cmd:真实命令（字符串_cmdReceived）:【/app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/bin/clang -v -Wp,-MD,arch/x86/boot/compressed/.misc.o.d -nostdinc -isystem /app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/lib/clang/15.0.0/include -I./arch/x86/include -I./arch/x86/include/generated -I./include -I./arch/x86/include/uapi -I./arch/x86/include/generated/uapi -I./include/uapi -I./include/generated/uapi -include ./include/linux/kconfig.h -D__KERNEL__ -Qunused-arguments -m32 -D__KERNEL__ -O1 -fno-strict-aliasing -fPIE -DDISABLE_BRANCH_PROFILING -march=i386 -mno-mmx -mno-sse -ffreestanding -fno-stack-protector -Wno-address-of-packed-member -Wa,-mrelax-relocations=no -DKBUILD_BASENAME="misc" -DKBUILD_MODNAME="misc" -c -o arch/x86/boot/compressed/misc.o arch/x86/boot/compressed/misc.c】
INFO:2024-01-09 09:24:33.435556@/bal/cmd-wrap/interceptor_util.py:66:execute_cmd:真实命令退出码,异常退出码:【1】
INFO:2024-01-09 09:24:33.435598@/bal/cmd-wrap/interceptor_util.py:71:execute_cmd:真实命令标准输出【】
INFO:2024-01-09 09:24:33.435673@/bal/cmd-wrap/interceptor_util.py:74:execute_cmd:真实命令错误输出【clang version 15.0.0 (git@github.com:llvm/llvm-project.git 4ba6a9c9f65bbc8bd06e3652cb20fd4dfc846137)
Target: i386-unknown-linux-gnu
Thread model: posix
InstalledDir: /app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/bin
Found candidate GCC installation: /usr/lib/gcc-cross/i686-linux-gnu/11
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/11
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/12
Selected GCC installation: /usr/lib/gcc-cross/i686-linux-gnu/11
Candidate multilib: .;@m32
Candidate multilib: 64;@m64
Candidate multilib: x32;@mx32
Selected multilib: .;@m32
 (in-process)
 "/app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/bin/clang-15" -cc1 -triple i386-unknown-linux-gnu -emit-obj -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name misc.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -relaxed-aliasing -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -ffreestanding -target-cpu i386 -target-feature -mmx -target-feature -sse -mllvm -treat-scalable-fixed-error-as-warning -debugger-tuning=gdb -v -fcoverage-compilation-dir=/bal/linux-stable -nostdsysteminc -nobuiltininc -resource-dir /app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/lib/clang/15.0.0 -dependency-file arch/x86/boot/compressed/.misc.o.d -MT arch/x86/boot/compressed/misc.o -sys-header-deps -isystem /app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/lib/clang/15.0.0/include -include ./include/linux/kconfig.h -I ./arch/x86/include -I ./arch/x86/include/generated -I ./include -I ./arch/x86/include/uapi -I ./arch/x86/include/generated/uapi -I ./include/uapi -I ./include/generated/uapi -D __KERNEL__ -D __KERNEL__ -D DISABLE_BRANCH_PROFILING -D "KBUILD_BASENAME=\"misc\"" -D "KBUILD_MODNAME=\"misc\"" -O1 -Wno-address-of-packed-member -fdebug-compilation-dir=/bal/linux-stable -ferror-limit 19 -fgnuc-version=4.2.1 -faddrsig -o arch/x86/boot/compressed/misc.o -x c arch/x86/boot/compressed/misc.c
clang -cc1 version 15.0.0 based upon LLVM 15.0.0 default target x86_64-unknown-linux-gnu
#include "..." search starts here:
#include <...> search starts here:
 ./arch/x86/include
 ./arch/x86/include/generated
 ./include
 ./arch/x86/include/uapi
 ./arch/x86/include/generated/uapi
 ./include/uapi
 ./include/generated/uapi
 /app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/lib/clang/15.0.0/include
End of search list.
arch/x86/boot/compressed/misc.c:1:9: warning: funcId_asm_inserted [-W#pragma-messages]
#pragma message("funcId_asm_inserted")
        ^
arch/x86/boot/compressed/misc.c:128:1: error: invalid operand for inline asm constraint 'i'  #文件 arch/x86/boot/compressed/misc.c 中的 函数 __putstr 报错
"jmp 0f \n\t"  
^
arch/x86/boot/compressed/misc.c:186:1: error: invalid operand for inline asm constraint 'i'   #文件 arch/x86/boot/compressed/misc.c 中的 函数 __puthex 报错
"jmp 0f \n\t"  
^
arch/x86/boot/compressed/misc.c:419:1: error: invalid operand for inline asm constraint 'i'   #文件 arch/x86/boot/compressed/misc.c 中的 函数 extract_kernel 报错
"jmp 0f \n\t"  
^
arch/x86/boot/compressed/misc.c:186:1: error: invalid operand for inline asm constraint 'i'
"jmp 0f \n\t"  
^
arch/x86/boot/compressed/misc.c:186:1: error: invalid operand for inline asm constraint 'i'
arch/x86/boot/compressed/misc.c:186:1: error: invalid operand for inline asm constraint 'i'
arch/x86/boot/compressed/misc.c:186:1: error: invalid operand for inline asm constraint 'i'
arch/x86/boot/compressed/misc.c:186:1: error: invalid operand for inline asm constraint 'i'
arch/x86/boot/compressed/misc.c:511:1: error: invalid operand for inline asm constraint 'i'   #文件 arch/x86/boot/compressed/misc.c 中的 函数 fortify_panic 报错
"jmp 0f \n\t"  
^
1 warning and 9 errors generated.
】
INFO:2024-01-09 09:24:33.435725@/bal/bin/clang:134:<module>:已释放日志文件/bal/g-0.log锁


```

##  misc.c 中 报错的函数

###  __putstr
```cpp
//第126行到第182行
void __putstr(const char *s)
{__asm__  __volatile__ (  
"jmp 0f \n\t"  
"or $0xFFFFFFFF,%%edi \n\t"  
"or $260,%%edi \n\t"  
"mov 1,%%edi \n\t"   
"or %0,%%edi \n\t"   
"0: \n\t"  
:  
:  "i"( __putstr )   
); /* filePath=arch/x86/boot/compressed/misc.c,line=108,column=1,abs_location_id=260,funcName=__putstr,srcFileId=1 non_static_non_inline_func*/
	int x, y, pos;
	char c;

	if (early_serial_base) {
		const char *str = s;
		while (*str) {
			if (*str == '\n')
				serial_putchar('\r');
			serial_putchar(*str++);
		}
	}

	if (lines == 0 || cols == 0)
		return;

	x = boot_params->screen_info.orig_x;
	y = boot_params->screen_info.orig_y;

	while ((c = *s++) != '\0') {
		if (c == '\n') {
			x = 0;
			if (++y >= lines) {
				scroll();
				y--;
			}
		} else {
			vidmem[(x + cols * y) * 2] = c;
			if (++x >= cols) {
				x = 0;
				if (++y >= lines) {
					scroll();
					y--;
				}
			}
		}
	}

	boot_params->screen_info.orig_x = x;
	boot_params->screen_info.orig_y = y;

	pos = (x + cols * y) * 2;	/* Update cursor position */
	outb(14, vidport);
	outb(0xff & (pos >> 9), vidport+1);
	outb(15, vidport);
	outb(0xff & (pos >> 1), vidport+1);
}
```

### __puthex

```cpp
//第184行到第208行
void __puthex(unsigned long value)
{__asm__  __volatile__ (  
"jmp 0f \n\t"  
"or $0xFFFFFFFF,%%edi \n\t"  
"or $261,%%edi \n\t"  
"mov 1,%%edi \n\t"   
"or %0,%%edi \n\t"   
"0: \n\t"  
:  
:  "i"( __puthex )   
); /* filePath=arch/x86/boot/compressed/misc.c,line=157,column=1,abs_location_id=261,funcName=__puthex,srcFileId=1 non_static_non_inline_func*/
	char alpha[2] = "0";
	int bits;

	for (bits = sizeof(value) * 8 - 4; bits >= 0; bits -= 4) {
		unsigned long digit = (value >> bits) & 0xf;

		if (digit < 0xA)
			alpha[0] = '0' + digit;
		else
			alpha[0] = 'a' + (digit - 0xA);

		__putstr(alpha);
	}
}
```

### extract_kernel
```cpp
//第413行到第507行
asmlinkage __visible void *extract_kernel(void *rmode, memptr heap,
				  unsigned char *input_data,
				  unsigned long input_len,
				  unsigned char *output,
				  unsigned long output_len)
{__asm__  __volatile__ (  
"jmp 0f \n\t"  
"or $0xFFFFFFFF,%%edi \n\t"  
"or $265,%%edi \n\t"  
"mov 1,%%edi \n\t"   
"or %0,%%edi \n\t"   
"0: \n\t"  
:  
:  "i"( extract_kernel )   
); /* filePath=arch/x86/boot/compressed/misc.c,line=354,column=1,abs_location_id=265,funcName=extract_kernel,srcFileId=1 non_static_non_inline_func*/
	const unsigned long kernel_total_size = VO__end - VO__text;
	unsigned long virt_addr = LOAD_PHYSICAL_ADDR;

	/* Retain x86 boot parameters pointer passed from startup_32/64. */
	boot_params = rmode;

	/* Clear flags intended for solely in-kernel use. */
	boot_params->hdr.loadflags &= ~KASLR_FLAG;

	sanitize_boot_params(boot_params);

	if (boot_params->screen_info.orig_video_mode == 7) {
		vidmem = (char *) 0xb0000;
		vidport = 0x3b4;
	} else {
		vidmem = (char *) 0xb8000;
		vidport = 0x3d4;
	}

	lines = boot_params->screen_info.orig_video_lines;
	cols = boot_params->screen_info.orig_video_cols;

	console_init();
	debug_putstr("early console in extract_kernel\n");

	if (IS_ENABLED(CONFIG_X86_5LEVEL) && !l5_supported()) {
		error("This linux kernel as configured requires 5-level paging\n"
			"This CPU does not support the required 'cr4.la57' feature\n"
			"Unable to boot - please use a kernel appropriate for your CPU\n");
	}

	free_mem_ptr     = heap;	/* Heap */
	free_mem_end_ptr = heap + BOOT_HEAP_SIZE;

	/* Report initial kernel position details. */
	debug_putaddr(input_data);
	debug_putaddr(input_len);
	debug_putaddr(output);
	debug_putaddr(output_len);
	debug_putaddr(kernel_total_size);

	/*
	 * The memory hole needed for the kernel is the larger of either
	 * the entire decompressed kernel plus relocation table, or the
	 * entire decompressed kernel plus .bss and .brk sections.
	 */
	choose_random_location((unsigned long)input_data, input_len,
				(unsigned long *)&output,
				max(output_len, kernel_total_size),
				&virt_addr);

	/* Validate memory location choices. */
	if ((unsigned long)output & (MIN_KERNEL_ALIGN - 1))
		error("Destination physical address inappropriately aligned");
	if (virt_addr & (MIN_KERNEL_ALIGN - 1))
		error("Destination virtual address inappropriately aligned");
#ifdef CONFIG_X86_64
	if (heap > 0x3fffffffffffUL)
		error("Destination address too large");
	if (virt_addr + max(output_len, kernel_total_size) > KERNEL_IMAGE_SIZE)
		error("Destination virtual address is beyond the kernel mapping area");
#else
	if (heap > ((-__PAGE_OFFSET-(128<<20)-1) & 0x7fffffff))
		error("Destination address too large");
#endif
#ifndef CONFIG_RELOCATABLE
	if ((unsigned long)output != LOAD_PHYSICAL_ADDR)
		error("Destination address does not match LOAD_PHYSICAL_ADDR");
	if (virt_addr != LOAD_PHYSICAL_ADDR)
		error("Destination virtual address changed when not relocatable");
#endif

	debug_putstr("\nDecompressing Linux... ");
	__decompress(input_data, input_len, NULL, NULL, output, output_len,
			NULL, error);
	parse_elf(output);
	handle_relocations(output, output_len, virt_addr);
	debug_putstr("done.\nBooting the kernel.\n");
	return output;
}
```

###  fortify_panic

```cpp
//第509行到第521行
void fortify_panic(const char *name)
{__asm__  __volatile__ (  
"jmp 0f \n\t"  
"or $0xFFFFFFFF,%%edi \n\t"  
"or $266,%%edi \n\t"  
"mov 1,%%edi \n\t"   
"or %0,%%edi \n\t"   
"0: \n\t"  
:  
:  "i"( fortify_panic )   
); /* filePath=arch/x86/boot/compressed/misc.c,line=437,column=1,abs_location_id=266,funcName=fortify_panic,srcFileId=1 non_static_non_inline_func*/
	error("detected buffer overflow");
}
```