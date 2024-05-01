#include "code-gen.h"




void CodeGenerator::GenCode(QWORD address, std::vector<QWORD> args) {
	//10 bytes per movabs, 1 for rax for  and the rest for args
	//2 bytes for calling rax, 1 for ret
	size_t code_size = 10 * (args.size() + 1) + 2 + 1;
	int arg_idx = 0;

	code_block = (unsigned char*)AllocRWX(code_size);
	size_t write_offset = 0;
	short idx_ins;
	//first save args in registers, use movabs
	for (QWORD qword : args) {
		if (arg_idx >= 4)
			break;
		idx_ins = ArgIdxToRegister(arg_idx);
		WriteToBlock(code_block, write_offset, (UCHAR*)&idx_ins, 2);
		WriteToBlock(code_block, write_offset, (UCHAR*)&qword, 8);
		arg_idx++;
	}
	idx_ins = 0xb848;
	//mov rax, address
	WriteMovabsRAX(code_block, write_offset, address);
	//call rax
	idx_ins = 0xd0ff;
	WriteToBlock(code_block, write_offset, (UCHAR*)&idx_ins, 2);
	//ret
	WriteToBlock(code_block, write_offset, 0xc3);

}