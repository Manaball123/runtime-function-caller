#include "code-gen.h"




void CodeGenerator::GenCode(QWORD address, const std::vector<QWORD>& args, const std::vector<ArgType>& arg_types) {
	//10 bytes per movabs, 1 for rax for  and the rest for args
	//2 bytes for calling rax, 1 for ret

	//nvm i dont wanna bother, if someone does feel free to calculate
	size_t code_size = 0x1000;
	int arg_idx = 0;

	code_buf = (UCHAR*)AllocRWX(code_size);


	//WRITING PHASE:
	WritePushRSP();
	

	//first save args in registers, use movabs
	for (int i = 0; i < args.size(); i++) {
		if (i >= 4)
			break;
		WriteArgToRegister(i, args[i], arg_types[i]);

	}

	if (args.size() > 4) {
		for (int i = args.size() - 1; i >= 4; i--)
			WriteArgToStack(args[i], arg_types[i]);
	}
	//shadowspace
	WriteSubRsp(32);
	//mov rax, address
	WriteMovabsRAX(address);
	//call rax
	WriteCallRAX();
	//remove shadowspace
	WriteAddRsp(32);
	if (args.size() - 4 > 0)
		WriteAddRsp((args.size() - 4) * 8);
	WritePopRSP();
	//ret
	WriteByte(0xc3);

}