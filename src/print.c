#include "woody.h"

void print_file(t_env *env)
{
	print_ehdr(env, &env->elf.header);
	print_phdr_header();
	for (t_segment_list *segment = env->elf.segments; segment; segment = segment->next)
		print_phdr(env, &segment->header);
	print_shdr_header();
	int i = 0;
	for (t_section_list *section = env->elf.sections; section; section = section->next)
		print_shdr(env, &section->header, i++);
}

void print_ehdr(t_env *env, Elf64_Ehdr *hdr)
{
	(void)env;
	printf("ELF Header:\n");
	printf("  Magic:   %2X %2X %2X %2X\n", hdr->e_ident[EI_MAG0], hdr->e_ident[EI_MAG1], hdr->e_ident[EI_MAG2], hdr->e_ident[EI_MAG3]);
	printf("  %-35s%s\n", "Class:", hdr->e_ident[EI_CLASS] == 1 ? "ELF32" : "ELF64");
	printf("  %-35s%s\n", "Data:", hdr->e_ident[EI_DATA] == 1 ? "little endian" : "big endian");
	printf("  %-35s%d\n", "Version:", hdr->e_ident[EI_VERSION]);
	printf("  %-35s%d\n", "OS/ABI:", hdr->e_ident[EI_OSABI]);
	printf("  %-35s%d\n", "ABI version:", hdr->e_ident[EI_ABIVERSION]);
	printf("  %-35s%d\n", "Type:", hdr->e_type);
	printf("  %-35s%d\n", "Machine:",  hdr->e_machine);
	printf("  %-35s0x%x\n", "Version:", hdr->e_version);
	printf("  %-35s0x%lx\n", "Entry point adress:", hdr->e_entry);
	printf("  %-35s%ld\n", "Start of program headers:", hdr->e_phoff);
	printf("  %-35s%ld\n", "Start of section headers:", hdr->e_shoff);
	printf("  %-35s%x\n", "Flags:", hdr->e_flags);
	printf("  %-35s%d (bytes)\n", "Size of this header:", hdr->e_ehsize);
	printf("  %-35s%d (bytes)\n", "Size of program headers:", hdr->e_phentsize);
	printf("  %-35s%d\n", "Number of program headers:", hdr->e_phnum);
	printf("  %-35s%d (bytes)\n", "Size of section headers:", hdr->e_shentsize);
	printf("  %-35s%d\n", "Number of section headers:", hdr->e_shnum);
	printf("  %-35s%d\n", "Section header string table index:", hdr->e_shstrndx);
}

void print_phdr_header()
{
	printf("\n");
	printf("Program Headers:\n");
	printf("  %-14s %-18s %-18s %-20s\n", "Type", "Offset", "VirtAddr", "PhysAddr");
	printf("  %-14s %-18s %-18s %-8s%s\n", "", "FileSiz", "MemSiz", " Flags", "Align");
}

void print_phdr(t_env *env, Elf64_Phdr *hdr)
{
	(void)env;
	char flags[4];
	flags[3] = '\0';
	flags[2] = (hdr->p_flags & 0x1) ? 'E' : ' ';
	flags[1] = (hdr->p_flags & 0x2) ? 'W' : ' ';
	flags[0] = (hdr->p_flags & 0x4) ? 'R' : ' ';
	char *type = "UNKNOWN";
	if (hdr->p_type == PT_NULL)
		type = "NULL";
	else if (hdr->p_type == PT_LOAD)
		type = "LOAD";
	else if (hdr->p_type == PT_DYNAMIC)
		type = "DYNAMIC";
	else if (hdr->p_type == PT_INTERP)
		type = "INTERP";
	else if (hdr->p_type == PT_NOTE)
		type = "NOTE";
	else if (hdr->p_type == PT_SHLIB)
		type = "SHLIB";
	else if (hdr->p_type == PT_PHDR)
		type = "PHDR";
	else if (hdr->p_type == PT_TLS)
		type = "TLS";
	else if (hdr->p_type == PT_NUM)
		type = "NUM";
	else if (hdr->p_type == PT_LOOS)
		type = "LOOS";
	else if (hdr->p_type == PT_GNU_EH_FRAME)
		type = "GNU_EH_FRAME";
	else if (hdr->p_type == PT_GNU_STACK)
		type = "GNU_STACK";
	else if (hdr->p_type == PT_GNU_RELRO)
		type = "GNO_RELRO";
	else if (hdr->p_type == PT_LOSUNW)
		type = "LOSUNW";
	else if (hdr->p_type == PT_SUNWBSS)
		type = "SUNWBSS";
	else if (hdr->p_type == PT_SUNWSTACK)
		type = "SUNWSTACK";
	else if (hdr->p_type == PT_HISUNW)
		type = "HISUNW";
	else if (hdr->p_type == PT_HIOS)
		type = "HIOS";
	else if (hdr->p_type == PT_LOPROC)
		type = "LOPROC";
	else if (hdr->p_type == PT_HIPROC)
		type = "HIPROC";
	printf("  %-14s 0x%016lx 0x%016lx 0x%016lx\n", type, hdr->p_offset, hdr->p_vaddr, hdr->p_paddr);
	printf("  %-14s 0x%016lx 0x%016lx  %-6s 0x%lx\n", "", hdr->p_filesz, hdr->p_memsz, flags, hdr->p_align);
}

void print_shdr_header()
{
	printf("\n");
	printf("Section Headers:\n");
	printf("  [Nr] %-16s  %-16s  %-16s  %-8s\n", "Name", "Type", "Address", "Offset");
	printf("       %-16s  %-16s  %-7s%-6s%-5s %s\n", "Size", "EntSize", "Flags", "Link", "Info", "Align");
}

void print_shdr(t_env *env, Elf64_Shdr *hdr, int idx)
{
	char *type = "UNKNOWN";
	if (hdr->sh_type == SHT_NULL)
		type = "NULL";
	else if (hdr->sh_type == SHT_PROGBITS)
		type = "PROGBITS";
	else if (hdr->sh_type == SHT_SYMTAB)
		type = "SYMTAB";
	else if (hdr->sh_type == SHT_DYNSYM)
		type = "DYNSYM";
	else if (hdr->sh_type == SHT_GNU_HASH)
		type = "GNU_HASH";
	else if (hdr->sh_type == SHT_INIT_ARRAY)
		type = "INIT_ARRAY";
	else if (hdr->sh_type == SHT_FINI_ARRAY)
		type = "FINI_ARRAY";
	else if (hdr->sh_type == SHT_DYNAMIC)
		type = "DYNAMIC";
	else if (hdr->sh_type == SHT_NOBITS)
		type = "NOBITS";
	else if (hdr->sh_type == SHT_STRTAB)
		type = "STRTAB";
	else if (hdr->sh_type == SHT_NOTE)
		type = "NOTE";
	else if (hdr->sh_type == SHT_GNU_versym)
		type = "VERSYM";
	else if (hdr->sh_type == SHT_GNU_verneed)
		type = "VERNEED";
	else if (hdr->sh_type == SHT_RELA)
		type = "RELA";
	char *name = "";
	int i = 0;
	for (t_section_list *section = env->elf.sections; section; section = section->next)
	{
		if (i++ == env->elf.header.e_shstrndx)
		{
			if (hdr->sh_name > section->header.sh_size)
				break;
			name = (char*)env->bin + section->header.sh_offset + hdr->sh_name;
			break;
		}
	}
	printf("  [%2d] %-16.16s  %-16s  %016lx  %08lx\n", idx, name, type, hdr->sh_addr, hdr->sh_offset);
	printf("       %016lx  %016lx  %-7ld%-6d%-5d %ld\n", hdr->sh_size, hdr->sh_entsize, hdr->sh_flags, hdr->sh_link, hdr->sh_info, hdr->sh_addralign);
}
