//
// Created by rfleuryleveso on 20/04/23.
//

#include "idt.h"
#include "../print.h"
#include "inttypes.h"
#include "../graphics/text_output_manager.h"
#include "../console/console_manager.h"
#include "../pit/pit.h"

__attribute__((aligned(0x10)))
idt_entry_t idt_entries[256];
volatile uint64_t last_called_isr = 0;

idt_ptr_t idt_ptr;

#pragma GCC push_options
#pragma GCC optimize ("O0")
// Interrupt handler routine for IRQ0

struct regs {
	uint64_t eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */
};

#pragma GCC pop_options

// Print information about an IDT entry
void print_idt_entry (idt_entry_t *entry)
{
  // printf("IDT entry %d:\n", i);
  printf ("Offset: %016lx\n", ((uint64_t)entry->offset_high << 32) | (entry->offset_mid << 16) | entry->offset_low);
  printf ("Segment selector: %04x\n", entry->selector);
  printf ("IST: %d\n", entry->ist);
  printf ("Flags: %02x\n", entry->type_attr);
}

void register_idt_entry (uint8_t index, uint8_t type_attr, uint64_t handler)
{
  idt_entries[index].offset_low = (uint16_t)(handler & 0xFFFF);
  idt_entries[index].selector = 0x08; // code segment selector
  idt_entries[index].ist = 0;
  idt_entries[index].type_attr = type_attr; // interrupt gate type and attributes for 64-bit interrupts
  idt_entries[index].offset_mid = (uint16_t)((handler >> 16) & 0xFFFF);
  idt_entries[index].offset_high = (uint32_t)((handler >> 32) & 0xFFFFFFFF);
  idt_entries[index].zero = 0;
}
// Function to initialize the IDT
void init_idt ()
{
  last_called_isr = 0;
  memset (&idt_entries[0], sizeof (idt_entries), 0);
  register_idt_entry (0, 0x8E, (uint64_t)&isr_handler_0);
  register_idt_entry (1, 0x8E, (uint64_t)&isr_handler_1);
  register_idt_entry (2, 0x8E, (uint64_t)&isr_handler_2);
  register_idt_entry (3, 0x8E, (uint64_t)&isr_handler_3);
  register_idt_entry (4, 0x8E, (uint64_t)&isr_handler_4);
  register_idt_entry (5, 0x8E, (uint64_t)&isr_handler_5);
  register_idt_entry (6, 0x8E, (uint64_t)&isr_handler_6);
  register_idt_entry (7, 0x8E, (uint64_t)&isr_handler_7);
  register_idt_entry (8, 0x8E, (uint64_t)&isr_handler_8);
  register_idt_entry (9, 0x8E, (uint64_t)&isr_handler_9);
  register_idt_entry (10, 0x8E, (uint64_t)&isr_handler_10);
  register_idt_entry (11, 0x8E, (uint64_t)&isr_handler_11);
  register_idt_entry (12, 0x8E, (uint64_t)&isr_handler_12);
  register_idt_entry (13, 0x8E, (uint64_t)&isr_handler_13);
  register_idt_entry (14, 0x8E, (uint64_t)&isr_handler_14);
  register_idt_entry (15, 0x8E, (uint64_t)&isr_handler_15);
  register_idt_entry (16, 0x8E, (uint64_t)&isr_handler_16);
  register_idt_entry (17, 0x8E, (uint64_t)&isr_handler_17);
  register_idt_entry (18, 0x8E, (uint64_t)&isr_handler_18);
  register_idt_entry (19, 0x8E, (uint64_t)&isr_handler_19);
  register_idt_entry (20, 0x8E, (uint64_t)&isr_handler_20);
  register_idt_entry (21, 0x8E, (uint64_t)&isr_handler_21);
  register_idt_entry (22, 0x8E, (uint64_t)&isr_handler_22);
  register_idt_entry (23, 0x8E, (uint64_t)&isr_handler_23);
  register_idt_entry (24, 0x8E, (uint64_t)&isr_handler_24);
  register_idt_entry (25, 0x8E, (uint64_t)&isr_handler_25);
  register_idt_entry (26, 0x8E, (uint64_t)&isr_handler_26);
  register_idt_entry (27, 0x8E, (uint64_t)&isr_handler_27);
  register_idt_entry (28, 0x8E, (uint64_t)&isr_handler_28);
  register_idt_entry (29, 0x8E, (uint64_t)&isr_handler_29);
  register_idt_entry (30, 0x8E, (uint64_t)&isr_handler_30);
  register_idt_entry (31, 0x8E, (uint64_t)&isr_handler_31);
  register_idt_entry (32, 0x8E, (uint64_t)&isr_handler_32);
  register_idt_entry (33, 0x8E, (uint64_t)&isr_handler_33);
  register_idt_entry (34, 0x8E, (uint64_t)&isr_handler_34);
  register_idt_entry (35, 0x8E, (uint64_t)&isr_handler_35);
  register_idt_entry (36, 0x8E, (uint64_t)&isr_handler_36);
  register_idt_entry (37, 0x8E, (uint64_t)&isr_handler_37);
  register_idt_entry (38, 0x8E, (uint64_t)&isr_handler_38);
  register_idt_entry (39, 0x8E, (uint64_t)&isr_handler_39);
  register_idt_entry (40, 0x8E, (uint64_t)&isr_handler_40);
  register_idt_entry (41, 0x8E, (uint64_t)&isr_handler_41);
  register_idt_entry (42, 0x8E, (uint64_t)&isr_handler_42);
  register_idt_entry (43, 0x8E, (uint64_t)&isr_handler_43);
  register_idt_entry (44, 0x8E, (uint64_t)&isr_handler_44);
  register_idt_entry (45, 0x8E, (uint64_t)&isr_handler_45);
  register_idt_entry (46, 0x8E, (uint64_t)&isr_handler_46);
  register_idt_entry (47, 0x8E, (uint64_t)&isr_handler_47);
  register_idt_entry (48, 0x8E, (uint64_t)&isr_handler_48);
  register_idt_entry (49, 0x8E, (uint64_t)&isr_handler_49);
  register_idt_entry (50, 0x8E, (uint64_t)&isr_handler_50);
  register_idt_entry (51, 0x8E, (uint64_t)&isr_handler_51);
  register_idt_entry (52, 0x8E, (uint64_t)&isr_handler_52);
  register_idt_entry (53, 0x8E, (uint64_t)&isr_handler_53);
  register_idt_entry (54, 0x8E, (uint64_t)&isr_handler_54);
  register_idt_entry (55, 0x8E, (uint64_t)&isr_handler_55);
  register_idt_entry (56, 0x8E, (uint64_t)&isr_handler_56);
  register_idt_entry (57, 0x8E, (uint64_t)&isr_handler_57);
  register_idt_entry (58, 0x8E, (uint64_t)&isr_handler_58);
  register_idt_entry (59, 0x8E, (uint64_t)&isr_handler_59);
  register_idt_entry (60, 0x8E, (uint64_t)&isr_handler_60);
  register_idt_entry (61, 0x8E, (uint64_t)&isr_handler_61);
  register_idt_entry (62, 0x8E, (uint64_t)&isr_handler_62);
  register_idt_entry (63, 0x8E, (uint64_t)&isr_handler_63);
  register_idt_entry (64, 0x8E, (uint64_t)&isr_handler_64);
  register_idt_entry (65, 0x8E, (uint64_t)&isr_handler_65);
  register_idt_entry (66, 0x8E, (uint64_t)&isr_handler_66);
  register_idt_entry (67, 0x8E, (uint64_t)&isr_handler_67);
  register_idt_entry (68, 0x8E, (uint64_t)&isr_handler_68);
  register_idt_entry (69, 0x8E, (uint64_t)&isr_handler_69);
  register_idt_entry (70, 0x8E, (uint64_t)&isr_handler_70);
  register_idt_entry (71, 0x8E, (uint64_t)&isr_handler_71);
  register_idt_entry (72, 0x8E, (uint64_t)&isr_handler_72);
  register_idt_entry (73, 0x8E, (uint64_t)&isr_handler_73);
  register_idt_entry (74, 0x8E, (uint64_t)&isr_handler_74);
  register_idt_entry (75, 0x8E, (uint64_t)&isr_handler_75);
  register_idt_entry (76, 0x8E, (uint64_t)&isr_handler_76);
  register_idt_entry (77, 0x8E, (uint64_t)&isr_handler_77);
  register_idt_entry (78, 0x8E, (uint64_t)&isr_handler_78);
  register_idt_entry (79, 0x8E, (uint64_t)&isr_handler_79);
  register_idt_entry (80, 0x8E, (uint64_t)&isr_handler_80);
  register_idt_entry (81, 0x8E, (uint64_t)&isr_handler_81);
  register_idt_entry (82, 0x8E, (uint64_t)&isr_handler_82);
  register_idt_entry (83, 0x8E, (uint64_t)&isr_handler_83);
  register_idt_entry (84, 0x8E, (uint64_t)&isr_handler_84);
  register_idt_entry (85, 0x8E, (uint64_t)&isr_handler_85);
  register_idt_entry (86, 0x8E, (uint64_t)&isr_handler_86);
  register_idt_entry (87, 0x8E, (uint64_t)&isr_handler_87);
  register_idt_entry (88, 0x8E, (uint64_t)&isr_handler_88);
  register_idt_entry (89, 0x8E, (uint64_t)&isr_handler_89);
  register_idt_entry (90, 0x8E, (uint64_t)&isr_handler_90);
  register_idt_entry (91, 0x8E, (uint64_t)&isr_handler_91);
  register_idt_entry (92, 0x8E, (uint64_t)&isr_handler_92);
  register_idt_entry (93, 0x8E, (uint64_t)&isr_handler_93);
  register_idt_entry (94, 0x8E, (uint64_t)&isr_handler_94);
  register_idt_entry (95, 0x8E, (uint64_t)&isr_handler_95);
  register_idt_entry (96, 0x8E, (uint64_t)&isr_handler_96);
  register_idt_entry (97, 0x8E, (uint64_t)&isr_handler_97);
  register_idt_entry (98, 0x8E, (uint64_t)&isr_handler_98);
  register_idt_entry (99, 0x8E, (uint64_t)&isr_handler_99);
  register_idt_entry (100, 0x8E, (uint64_t)&isr_handler_100);
  register_idt_entry (101, 0x8E, (uint64_t)&isr_handler_101);
  register_idt_entry (102, 0x8E, (uint64_t)&isr_handler_102);
  register_idt_entry (103, 0x8E, (uint64_t)&isr_handler_103);
  register_idt_entry (104, 0x8E, (uint64_t)&isr_handler_104);
  register_idt_entry (105, 0x8E, (uint64_t)&isr_handler_105);
  register_idt_entry (106, 0x8E, (uint64_t)&isr_handler_106);
  register_idt_entry (107, 0x8E, (uint64_t)&isr_handler_107);
  register_idt_entry (108, 0x8E, (uint64_t)&isr_handler_108);
  register_idt_entry (109, 0x8E, (uint64_t)&isr_handler_109);
  register_idt_entry (110, 0x8E, (uint64_t)&isr_handler_110);
  register_idt_entry (111, 0x8E, (uint64_t)&isr_handler_111);
  register_idt_entry (112, 0x8E, (uint64_t)&isr_handler_112);
  register_idt_entry (113, 0x8E, (uint64_t)&isr_handler_113);
  register_idt_entry (114, 0x8E, (uint64_t)&isr_handler_114);
  register_idt_entry (115, 0x8E, (uint64_t)&isr_handler_115);
  register_idt_entry (116, 0x8E, (uint64_t)&isr_handler_116);
  register_idt_entry (117, 0x8E, (uint64_t)&isr_handler_117);
  register_idt_entry (118, 0x8E, (uint64_t)&isr_handler_118);
  register_idt_entry (119, 0x8E, (uint64_t)&isr_handler_119);
  register_idt_entry (120, 0x8E, (uint64_t)&isr_handler_120);
  register_idt_entry (121, 0x8E, (uint64_t)&isr_handler_121);
  register_idt_entry (122, 0x8E, (uint64_t)&isr_handler_122);
  register_idt_entry (123, 0x8E, (uint64_t)&isr_handler_123);
  register_idt_entry (124, 0x8E, (uint64_t)&isr_handler_124);
  register_idt_entry (125, 0x8E, (uint64_t)&isr_handler_125);
  register_idt_entry (126, 0x8E, (uint64_t)&isr_handler_126);
  register_idt_entry (127, 0x8E, (uint64_t)&isr_handler_127);
  register_idt_entry (128, 0x8E, (uint64_t)&isr_handler_128);
  register_idt_entry (129, 0x8E, (uint64_t)&isr_handler_129);
  register_idt_entry (130, 0x8E, (uint64_t)&isr_handler_130);
  register_idt_entry (131, 0x8E, (uint64_t)&isr_handler_131);
  register_idt_entry (132, 0x8E, (uint64_t)&isr_handler_132);
  register_idt_entry (133, 0x8E, (uint64_t)&isr_handler_133);
  register_idt_entry (134, 0x8E, (uint64_t)&isr_handler_134);
  register_idt_entry (135, 0x8E, (uint64_t)&isr_handler_135);
  register_idt_entry (136, 0x8E, (uint64_t)&isr_handler_136);
  register_idt_entry (137, 0x8E, (uint64_t)&isr_handler_137);
  register_idt_entry (138, 0x8E, (uint64_t)&isr_handler_138);
  register_idt_entry (139, 0x8E, (uint64_t)&isr_handler_139);
  register_idt_entry (140, 0x8E, (uint64_t)&isr_handler_140);
  register_idt_entry (141, 0x8E, (uint64_t)&isr_handler_141);
  register_idt_entry (142, 0x8E, (uint64_t)&isr_handler_142);
  register_idt_entry (143, 0x8E, (uint64_t)&isr_handler_143);
  register_idt_entry (144, 0x8E, (uint64_t)&isr_handler_144);
  register_idt_entry (145, 0x8E, (uint64_t)&isr_handler_145);
  register_idt_entry (146, 0x8E, (uint64_t)&isr_handler_146);
  register_idt_entry (147, 0x8E, (uint64_t)&isr_handler_147);
  register_idt_entry (148, 0x8E, (uint64_t)&isr_handler_148);
  register_idt_entry (149, 0x8E, (uint64_t)&isr_handler_149);
  register_idt_entry (150, 0x8E, (uint64_t)&isr_handler_150);
  register_idt_entry (151, 0x8E, (uint64_t)&isr_handler_151);
  register_idt_entry (152, 0x8E, (uint64_t)&isr_handler_152);
  register_idt_entry (153, 0x8E, (uint64_t)&isr_handler_153);
  register_idt_entry (154, 0x8E, (uint64_t)&isr_handler_154);
  register_idt_entry (155, 0x8E, (uint64_t)&isr_handler_155);
  register_idt_entry (156, 0x8E, (uint64_t)&isr_handler_156);
  register_idt_entry (157, 0x8E, (uint64_t)&isr_handler_157);
  register_idt_entry (158, 0x8E, (uint64_t)&isr_handler_158);
  register_idt_entry (159, 0x8E, (uint64_t)&isr_handler_159);
  register_idt_entry (160, 0x8E, (uint64_t)&isr_handler_160);
  register_idt_entry (161, 0x8E, (uint64_t)&isr_handler_161);
  register_idt_entry (162, 0x8E, (uint64_t)&isr_handler_162);
  register_idt_entry (163, 0x8E, (uint64_t)&isr_handler_163);
  register_idt_entry (164, 0x8E, (uint64_t)&isr_handler_164);
  register_idt_entry (165, 0x8E, (uint64_t)&isr_handler_165);
  register_idt_entry (166, 0x8E, (uint64_t)&isr_handler_166);
  register_idt_entry (167, 0x8E, (uint64_t)&isr_handler_167);
  register_idt_entry (168, 0x8E, (uint64_t)&isr_handler_168);
  register_idt_entry (169, 0x8E, (uint64_t)&isr_handler_169);
  register_idt_entry (170, 0x8E, (uint64_t)&isr_handler_170);
  register_idt_entry (171, 0x8E, (uint64_t)&isr_handler_171);
  register_idt_entry (172, 0x8E, (uint64_t)&isr_handler_172);
  register_idt_entry (173, 0x8E, (uint64_t)&isr_handler_173);
  register_idt_entry (174, 0x8E, (uint64_t)&isr_handler_174);
  register_idt_entry (175, 0x8E, (uint64_t)&isr_handler_175);
  register_idt_entry (176, 0x8E, (uint64_t)&isr_handler_176);
  register_idt_entry (177, 0x8E, (uint64_t)&isr_handler_177);
  register_idt_entry (178, 0x8E, (uint64_t)&isr_handler_178);
  register_idt_entry (179, 0x8E, (uint64_t)&isr_handler_179);
  register_idt_entry (180, 0x8E, (uint64_t)&isr_handler_180);
  register_idt_entry (181, 0x8E, (uint64_t)&isr_handler_181);
  register_idt_entry (182, 0x8E, (uint64_t)&isr_handler_182);
  register_idt_entry (183, 0x8E, (uint64_t)&isr_handler_183);
  register_idt_entry (184, 0x8E, (uint64_t)&isr_handler_184);
  register_idt_entry (185, 0x8E, (uint64_t)&isr_handler_185);
  register_idt_entry (186, 0x8E, (uint64_t)&isr_handler_186);
  register_idt_entry (187, 0x8E, (uint64_t)&isr_handler_187);
  register_idt_entry (188, 0x8E, (uint64_t)&isr_handler_188);
  register_idt_entry (189, 0x8E, (uint64_t)&isr_handler_189);
  register_idt_entry (190, 0x8E, (uint64_t)&isr_handler_190);
  register_idt_entry (191, 0x8E, (uint64_t)&isr_handler_191);
  register_idt_entry (192, 0x8E, (uint64_t)&isr_handler_192);
  register_idt_entry (193, 0x8E, (uint64_t)&isr_handler_193);
  register_idt_entry (194, 0x8E, (uint64_t)&isr_handler_194);
  register_idt_entry (195, 0x8E, (uint64_t)&isr_handler_195);
  register_idt_entry (196, 0x8E, (uint64_t)&isr_handler_196);
  register_idt_entry (197, 0x8E, (uint64_t)&isr_handler_197);
  register_idt_entry (198, 0x8E, (uint64_t)&isr_handler_198);
  register_idt_entry (199, 0x8E, (uint64_t)&isr_handler_199);
  register_idt_entry (200, 0x8E, (uint64_t)&isr_handler_200);
  register_idt_entry (201, 0x8E, (uint64_t)&isr_handler_201);
  register_idt_entry (202, 0x8E, (uint64_t)&isr_handler_202);
  register_idt_entry (203, 0x8E, (uint64_t)&isr_handler_203);
  register_idt_entry (204, 0x8E, (uint64_t)&isr_handler_204);
  register_idt_entry (205, 0x8E, (uint64_t)&isr_handler_205);
  register_idt_entry (206, 0x8E, (uint64_t)&isr_handler_206);
  register_idt_entry (207, 0x8E, (uint64_t)&isr_handler_207);
  register_idt_entry (208, 0x8E, (uint64_t)&isr_handler_208);
  register_idt_entry (209, 0x8E, (uint64_t)&isr_handler_209);
  register_idt_entry (210, 0x8E, (uint64_t)&isr_handler_210);
  register_idt_entry (211, 0x8E, (uint64_t)&isr_handler_211);
  register_idt_entry (212, 0x8E, (uint64_t)&isr_handler_212);
  register_idt_entry (213, 0x8E, (uint64_t)&isr_handler_213);
  register_idt_entry (214, 0x8E, (uint64_t)&isr_handler_214);
  register_idt_entry (215, 0x8E, (uint64_t)&isr_handler_215);
  register_idt_entry (216, 0x8E, (uint64_t)&isr_handler_216);
  register_idt_entry (217, 0x8E, (uint64_t)&isr_handler_217);
  register_idt_entry (218, 0x8E, (uint64_t)&isr_handler_218);
  register_idt_entry (219, 0x8E, (uint64_t)&isr_handler_219);
  register_idt_entry (220, 0x8E, (uint64_t)&isr_handler_220);
  register_idt_entry (221, 0x8E, (uint64_t)&isr_handler_221);
  register_idt_entry (222, 0x8E, (uint64_t)&isr_handler_222);
  register_idt_entry (223, 0x8E, (uint64_t)&isr_handler_223);
  register_idt_entry (224, 0x8E, (uint64_t)&isr_handler_224);
  register_idt_entry (225, 0x8E, (uint64_t)&isr_handler_225);
  register_idt_entry (226, 0x8E, (uint64_t)&isr_handler_226);
  register_idt_entry (227, 0x8E, (uint64_t)&isr_handler_227);
  register_idt_entry (228, 0x8E, (uint64_t)&isr_handler_228);
  register_idt_entry (229, 0x8E, (uint64_t)&isr_handler_229);
  register_idt_entry (230, 0x8E, (uint64_t)&isr_handler_230);
  register_idt_entry (231, 0x8E, (uint64_t)&isr_handler_231);
  register_idt_entry (232, 0x8E, (uint64_t)&isr_handler_232);
  register_idt_entry (233, 0x8E, (uint64_t)&isr_handler_233);
  register_idt_entry (234, 0x8E, (uint64_t)&isr_handler_234);
  register_idt_entry (235, 0x8E, (uint64_t)&isr_handler_235);
  register_idt_entry (236, 0x8E, (uint64_t)&isr_handler_236);
  register_idt_entry (237, 0x8E, (uint64_t)&isr_handler_237);
  register_idt_entry (238, 0x8E, (uint64_t)&isr_handler_238);
  register_idt_entry (239, 0x8E, (uint64_t)&isr_handler_239);
  register_idt_entry (240, 0x8E, (uint64_t)&isr_handler_240);
  register_idt_entry (241, 0x8E, (uint64_t)&isr_handler_241);
  register_idt_entry (242, 0x8E, (uint64_t)&isr_handler_242);
  register_idt_entry (243, 0x8E, (uint64_t)&isr_handler_243);
  register_idt_entry (244, 0x8E, (uint64_t)&isr_handler_244);
  register_idt_entry (245, 0x8E, (uint64_t)&isr_handler_245);
  register_idt_entry (246, 0x8E, (uint64_t)&isr_handler_246);
  register_idt_entry (247, 0x8E, (uint64_t)&isr_handler_247);
  register_idt_entry (248, 0x8E, (uint64_t)&isr_handler_248);
  register_idt_entry (249, 0x8E, (uint64_t)&isr_handler_249);
  register_idt_entry (250, 0x8E, (uint64_t)&isr_handler_250);
  register_idt_entry (251, 0x8E, (uint64_t)&isr_handler_251);
  register_idt_entry (252, 0x8E, (uint64_t)&isr_handler_252);
  register_idt_entry (253, 0x8E, (uint64_t)&isr_handler_253);
  register_idt_entry (254, 0x8E, (uint64_t)&isr_handler_254);
  register_idt_entry (255, 0x8E, (uint64_t)&isr_handler_255);
// Load the IDT
  idt_ptr.base = (uint64_t)&idt_entries;
  // idt_ptr.size = sizeof(idt_entries) - 1;
  idt_ptr.size = 4095;

  printf ("IDT_PTR = %016"PRIx64 " IDT =%016"PRIx64 "\n", &idt_ptr, &idt_entries[0]);
  // Enable interrupts
  uint64_t ptr = (uint64_t)&idt_ptr; // get the pointer to your GDT table as a 32bit int
  asm volatile("movq %0,%%rax    \n\t"  //load the pointer int eax
			   "lidt (%%rax) "            //Set the CPU GDT pointer to your table
	  : : "r" (ptr));

  printf ("INTERRUPT TABLE UPDATED !! \n");

}
void __attribute__((optimize("O0"))) interrupt_handler_0 ()
{
  printf (">>>>>>>> INTERRUPT 0 ! \n");
  outb (0x20, 0);
  outb (0xa0, 0);
}
void __attribute__((optimize("O0"))) interrupt_handler_1 ()
{
  printf (">>>>>>>> INTERRUPT 1 ! \n");
  outb (0x20, 1);
  outb (0xa0, 1);
}
void __attribute__((optimize("O0"))) interrupt_handler_2 ()
{
  printf (">>>>>>>> INTERRUPT 2 ! \n");
  outb (0x20, 2);
  outb (0xa0, 2);
}
void __attribute__((optimize("O0"))) interrupt_handler_3 ()
{
  printf (">>>>>>>> INTERRUPT 3 ! \n");
  outb (0x20, 3);
  outb (0xa0, 3);
}
void __attribute__((optimize("O0"))) interrupt_handler_4 ()
{
  printf (">>>>>>>> INTERRUPT 4 ! \n");
  outb (0x20, 4);
  outb (0xa0, 4);
}
void __attribute__((optimize("O0"))) interrupt_handler_5 ()
{
  printf (">>>>>>>> INTERRUPT 5 ! \n");
  outb (0x20, 5);
  outb (0xa0, 5);
}
void __attribute__((optimize("O0"))) interrupt_handler_6 ()
{
  printf (">>>>>>>> INTERRUPT 6 ! \n");
  outb (0x20, 6);
  outb (0xa0, 6);
}
void __attribute__((optimize("O0"))) interrupt_handler_7 ()
{
  printf (">>>>>>>> INTERRUPT 7 ! \n");
  outb (0x20, 7);
  outb (0xa0, 7);
}
void __attribute__((optimize("O0"))) interrupt_handler_8 ()
{
  printf (">>>>>>>> INTERRUPT 8 ! \n");
  outb (0x20, 8);
  outb (0xa0, 8);
}
void __attribute__((optimize("O0"))) interrupt_handler_9 ()
{
  printf (">>>>>>>> INTERRUPT 9 ! \n");
  outb (0x20, 9);
  outb (0xa0, 9);
}
void __attribute__((optimize("O0"))) interrupt_handler_10 ()
{
  printf (">>>>>>>> INTERRUPT 10 ! \n");
  outb (0x20, 10);
  outb (0xa0, 10);
}
void __attribute__((optimize("O0"))) interrupt_handler_11 ()
{
  printf (">>>>>>>> INTERRUPT 11 ! \n");
  outb (0x20, 11);
  outb (0xa0, 11);
}
void __attribute__((optimize("O0"))) interrupt_handler_12 ()
{
  printf (">>>>>>>> INTERRUPT 12 ! \n");
  outb (0x20, 12);
  outb (0xa0, 12);
}
void __attribute__((optimize("O0"))) interrupt_handler_13 ()
{
  printf (">>>>>>>> INTERRUPT 13 ! \n");
  outb (0x20, 13);
  outb (0xa0, 13);
}
void __attribute__((optimize("O0"))) interrupt_handler_14 ()
{
  printf (">>>>>>>> INTERRUPT 14 ! \n");
  outb (0x20, 14);
  outb (0xa0, 14);
}
void __attribute__((optimize("O0"))) interrupt_handler_15 ()
{
  printf (">>>>>>>> INTERRUPT 15 ! \n");
  outb (0x20, 15);
  outb (0xa0, 15);
}
void __attribute__((optimize("O0"))) interrupt_handler_16 ()
{
  printf (">>>>>>>> INTERRUPT 16 ! \n");
  outb (0x20, 16);
  outb (0xa0, 16);
}
void __attribute__((optimize("O0"))) interrupt_handler_17 ()
{
  printf (">>>>>>>> INTERRUPT 17 ! \n");
  outb (0x20, 17);
  outb (0xa0, 17);
}
void __attribute__((optimize("O0"))) interrupt_handler_18 ()
{
  printf (">>>>>>>> INTERRUPT 18 ! \n");
  outb (0x20, 18);
  outb (0xa0, 18);
}
void __attribute__((optimize("O0"))) interrupt_handler_19 ()
{
  printf (">>>>>>>> INTERRUPT 19 ! \n");
  outb (0x20, 19);
  outb (0xa0, 19);
}
void __attribute__((optimize("O0"))) interrupt_handler_20 ()
{
  printf (">>>>>>>> INTERRUPT 20 ! \n");
  outb (0x20, 20);
  outb (0xa0, 20);
}
void __attribute__((optimize("O0"))) interrupt_handler_21 ()
{
  printf (">>>>>>>> INTERRUPT 21 ! \n");
  outb (0x20, 21);
  outb (0xa0, 21);
}
void __attribute__((optimize("O0"))) interrupt_handler_22 ()
{
  printf (">>>>>>>> INTERRUPT 22 ! \n");
  outb (0x20, 22);
  outb (0xa0, 22);
}
void __attribute__((optimize("O0"))) interrupt_handler_23 ()
{
  printf (">>>>>>>> INTERRUPT 23 ! \n");
  outb (0x20, 23);
  outb (0xa0, 23);
}
void __attribute__((optimize("O0"))) interrupt_handler_24 ()
{
  printf (">>>>>>>> INTERRUPT 24 ! \n");
  outb (0x20, 24);
  outb (0xa0, 24);
}
void __attribute__((optimize("O0"))) interrupt_handler_25 ()
{
  printf (">>>>>>>> INTERRUPT 25 ! \n");
  outb (0x20, 25);
  outb (0xa0, 25);
}
void __attribute__((optimize("O0"))) interrupt_handler_26 ()
{
  printf (">>>>>>>> INTERRUPT 26 ! \n");
  outb (0x20, 26);
  outb (0xa0, 26);
}
void __attribute__((optimize("O0"))) interrupt_handler_27 ()
{
  printf (">>>>>>>> INTERRUPT 27 ! \n");
  outb (0x20, 27);
  outb (0xa0, 27);
}
void __attribute__((optimize("O0"))) interrupt_handler_28 ()
{
  printf (">>>>>>>> INTERRUPT 28 ! \n");
  outb (0x20, 28);
  outb (0xa0, 28);
}
void __attribute__((optimize("O0"))) interrupt_handler_29 ()
{
  printf (">>>>>>>> INTERRUPT 29 ! \n");
  outb (0x20, 29);
  outb (0xa0, 29);
}
void __attribute__((optimize("O0"))) interrupt_handler_30 ()
{
  printf (">>>>>>>> INTERRUPT 30 ! \n");
  outb (0x20, 30);
  outb (0xa0, 30);
}
void __attribute__((optimize("O0"))) interrupt_handler_31 ()
{
  printf (">>>>>>>> INTERRUPT 31 ! \n");
  outb (0x20, 31);
  outb (0xa0, 31);
}
void __attribute__((optimize("O0"))) interrupt_handler_32 ()
{
  pit_counter_1000hz++;
  scheduler_tick();
  outb (0x20, 32);
  outb (0xa0, 32);
}
void __attribute__((optimize("O0"))) interrupt_handler_33 ()
{
  outb (0x20, 33);
  outb (0xa0, 33);

  uint8_t scan_code = inb (0x60);
  console_manager_keyboard_callback(scan_code);

}
void __attribute__((optimize("O0"))) interrupt_handler_34 ()
{
  printf (">>>>>>>> INTERRUPT 34 ! \n");
  outb (0x20, 34);
  outb (0xa0, 34);
}
void __attribute__((optimize("O0"))) interrupt_handler_35 ()
{
  printf (">>>>>>>> INTERRUPT 35 ! \n");
  outb (0x20, 35);
  outb (0xa0, 35);
}
void __attribute__((optimize("O0"))) interrupt_handler_36 ()
{
  printf (">>>>>>>> INTERRUPT 36 ! \n");
  outb (0x20, 36);
  outb (0xa0, 36);
}
void __attribute__((optimize("O0"))) interrupt_handler_37 ()
{
  printf (">>>>>>>> INTERRUPT 37 ! \n");
  outb (0x20, 37);
  outb (0xa0, 37);
}
void __attribute__((optimize("O0"))) interrupt_handler_38 ()
{
  printf (">>>>>>>> INTERRUPT 38 ! \n");
  outb (0x20, 38);
  outb (0xa0, 38);
}
void __attribute__((optimize("O0"))) interrupt_handler_39 ()
{
  printf (">>>>>>>> INTERRUPT 39 ! \n");
  outb (0x20, 39);
  outb (0xa0, 39);
}
void __attribute__((optimize("O0"))) interrupt_handler_40 ()
{
  printf (">>>>>>>> INTERRUPT 40 ! \n");
  outb (0x20, 40);
  outb (0xa0, 40);
}
void __attribute__((optimize("O0"))) interrupt_handler_41 ()
{
  printf (">>>>>>>> INTERRUPT 41 ! \n");
  outb (0x20, 41);
  outb (0xa0, 41);
}
void __attribute__((optimize("O0"))) interrupt_handler_42 ()
{
  printf (">>>>>>>> INTERRUPT 42 ! \n");
  outb (0x20, 42);
  outb (0xa0, 42);
}
void __attribute__((optimize("O0"))) interrupt_handler_43 ()
{
  printf (">>>>>>>> INTERRUPT 43 ! \n");
  outb (0x20, 43);
  outb (0xa0, 43);
}
void __attribute__((optimize("O0"))) interrupt_handler_44 ()
{
  printf (">>>>>>>> INTERRUPT 44 ! \n");
  outb (0x20, 44);
  outb (0xa0, 44);
}
void __attribute__((optimize("O0"))) interrupt_handler_45 ()
{
  printf (">>>>>>>> INTERRUPT 45 ! \n");
  outb (0x20, 45);
  outb (0xa0, 45);
}
void __attribute__((optimize("O0"))) interrupt_handler_46 ()
{
  printf (">>>>>>>> INTERRUPT 46 ! \n");
  outb (0x20, 46);
  outb (0xa0, 46);
}
void __attribute__((optimize("O0"))) interrupt_handler_47 ()
{
  printf (">>>>>>>> INTERRUPT 47 ! \n");
  outb (0x20, 47);
  outb (0xa0, 47);
}
void __attribute__((optimize("O0"))) interrupt_handler_48 ()
{
  printf (">>>>>>>> INTERRUPT 48 ! \n");
  outb (0x20, 48);
  outb (0xa0, 48);
}
void __attribute__((optimize("O0"))) interrupt_handler_49 ()
{
  printf (">>>>>>>> INTERRUPT 49 ! \n");
  outb (0x20, 49);
  outb (0xa0, 49);
}
void __attribute__((optimize("O0"))) interrupt_handler_50 ()
{
  printf (">>>>>>>> INTERRUPT 50 ! \n");
  outb (0x20, 50);
  outb (0xa0, 50);
}
void __attribute__((optimize("O0"))) interrupt_handler_51 ()
{
  printf (">>>>>>>> INTERRUPT 51 ! \n");
  outb (0x20, 51);
  outb (0xa0, 51);
}
void __attribute__((optimize("O0"))) interrupt_handler_52 ()
{
  printf (">>>>>>>> INTERRUPT 52 ! \n");
  outb (0x20, 52);
  outb (0xa0, 52);
}
void __attribute__((optimize("O0"))) interrupt_handler_53 ()
{
  printf (">>>>>>>> INTERRUPT 53 ! \n");
  outb (0x20, 53);
  outb (0xa0, 53);
}
void __attribute__((optimize("O0"))) interrupt_handler_54 ()
{
  printf (">>>>>>>> INTERRUPT 54 ! \n");
  outb (0x20, 54);
  outb (0xa0, 54);
}
void __attribute__((optimize("O0"))) interrupt_handler_55 ()
{
  printf (">>>>>>>> INTERRUPT 55 ! \n");
  outb (0x20, 55);
  outb (0xa0, 55);
}
void __attribute__((optimize("O0"))) interrupt_handler_56 ()
{
  printf (">>>>>>>> INTERRUPT 56 ! \n");
  outb (0x20, 56);
  outb (0xa0, 56);
}
void __attribute__((optimize("O0"))) interrupt_handler_57 ()
{
  printf (">>>>>>>> INTERRUPT 57 ! \n");
  outb (0x20, 57);
  outb (0xa0, 57);
}
void __attribute__((optimize("O0"))) interrupt_handler_58 ()
{
  printf (">>>>>>>> INTERRUPT 58 ! \n");
  outb (0x20, 58);
  outb (0xa0, 58);
}
void __attribute__((optimize("O0"))) interrupt_handler_59 ()
{
  printf (">>>>>>>> INTERRUPT 59 ! \n");
  outb (0x20, 59);
  outb (0xa0, 59);
}
void __attribute__((optimize("O0"))) interrupt_handler_60 ()
{
  printf (">>>>>>>> INTERRUPT 60 ! \n");
  outb (0x20, 60);
  outb (0xa0, 60);
}
void __attribute__((optimize("O0"))) interrupt_handler_61 ()
{
  printf (">>>>>>>> INTERRUPT 61 ! \n");
  outb (0x20, 61);
  outb (0xa0, 61);
}
void __attribute__((optimize("O0"))) interrupt_handler_62 ()
{
  printf (">>>>>>>> INTERRUPT 62 ! \n");
  outb (0x20, 62);
  outb (0xa0, 62);
}
void __attribute__((optimize("O0"))) interrupt_handler_63 ()
{
  printf (">>>>>>>> INTERRUPT 63 ! \n");
  outb (0x20, 63);
  outb (0xa0, 63);
}
void __attribute__((optimize("O0"))) interrupt_handler_64 ()
{
  printf (">>>>>>>> INTERRUPT 64 ! \n");
  outb (0x20, 64);
  outb (0xa0, 64);
}
void __attribute__((optimize("O0"))) interrupt_handler_65 ()
{
  printf (">>>>>>>> INTERRUPT 65 ! \n");
  outb (0x20, 65);
  outb (0xa0, 65);
}
void __attribute__((optimize("O0"))) interrupt_handler_66 ()
{
  printf (">>>>>>>> INTERRUPT 66 ! \n");
  outb (0x20, 66);
  outb (0xa0, 66);
}
void __attribute__((optimize("O0"))) interrupt_handler_67 ()
{
  printf (">>>>>>>> INTERRUPT 67 ! \n");
  outb (0x20, 67);
  outb (0xa0, 67);
}
void __attribute__((optimize("O0"))) interrupt_handler_68 ()
{
  printf (">>>>>>>> INTERRUPT 68 ! \n");
  outb (0x20, 68);
  outb (0xa0, 68);
}
void __attribute__((optimize("O0"))) interrupt_handler_69 ()
{
  printf (">>>>>>>> INTERRUPT 69 ! \n");
  outb (0x20, 69);
  outb (0xa0, 69);
}
void __attribute__((optimize("O0"))) interrupt_handler_70 ()
{
  printf (">>>>>>>> INTERRUPT 70 ! \n");
  outb (0x20, 70);
  outb (0xa0, 70);
}
void __attribute__((optimize("O0"))) interrupt_handler_71 ()
{
  printf (">>>>>>>> INTERRUPT 71 ! \n");
  outb (0x20, 71);
  outb (0xa0, 71);
}
void __attribute__((optimize("O0"))) interrupt_handler_72 ()
{
  printf (">>>>>>>> INTERRUPT 72 ! \n");
  outb (0x20, 72);
  outb (0xa0, 72);
}
void __attribute__((optimize("O0"))) interrupt_handler_73 ()
{
  printf (">>>>>>>> INTERRUPT 73 ! \n");
  outb (0x20, 73);
  outb (0xa0, 73);
}
void __attribute__((optimize("O0"))) interrupt_handler_74 ()
{
  printf (">>>>>>>> INTERRUPT 74 ! \n");
  outb (0x20, 74);
  outb (0xa0, 74);
}
void __attribute__((optimize("O0"))) interrupt_handler_75 ()
{
  printf (">>>>>>>> INTERRUPT 75 ! \n");
  outb (0x20, 75);
  outb (0xa0, 75);
}
void __attribute__((optimize("O0"))) interrupt_handler_76 ()
{
  printf (">>>>>>>> INTERRUPT 76 ! \n");
  outb (0x20, 76);
  outb (0xa0, 76);
}
void __attribute__((optimize("O0"))) interrupt_handler_77 ()
{
  printf (">>>>>>>> INTERRUPT 77 ! \n");
  outb (0x20, 77);
  outb (0xa0, 77);
}
void __attribute__((optimize("O0"))) interrupt_handler_78 ()
{
  printf (">>>>>>>> INTERRUPT 78 ! \n");
  outb (0x20, 78);
  outb (0xa0, 78);
}
void __attribute__((optimize("O0"))) interrupt_handler_79 ()
{
  printf (">>>>>>>> INTERRUPT 79 ! \n");
  outb (0x20, 79);
  outb (0xa0, 79);
}
void __attribute__((optimize("O0"))) interrupt_handler_80 ()
{
  printf (">>>>>>>> INTERRUPT 80 ! \n");
  outb (0x20, 80);
  outb (0xa0, 80);
}
void __attribute__((optimize("O0"))) interrupt_handler_81 ()
{
  printf (">>>>>>>> INTERRUPT 81 ! \n");
  outb (0x20, 81);
  outb (0xa0, 81);
}
void __attribute__((optimize("O0"))) interrupt_handler_82 ()
{
  printf (">>>>>>>> INTERRUPT 82 ! \n");
  outb (0x20, 82);
  outb (0xa0, 82);
}
void __attribute__((optimize("O0"))) interrupt_handler_83 ()
{
  printf (">>>>>>>> INTERRUPT 83 ! \n");
  outb (0x20, 83);
  outb (0xa0, 83);
}
void __attribute__((optimize("O0"))) interrupt_handler_84 ()
{
  printf (">>>>>>>> INTERRUPT 84 ! \n");
  outb (0x20, 84);
  outb (0xa0, 84);
}
void __attribute__((optimize("O0"))) interrupt_handler_85 ()
{
  printf (">>>>>>>> INTERRUPT 85 ! \n");
  outb (0x20, 85);
  outb (0xa0, 85);
}
void __attribute__((optimize("O0"))) interrupt_handler_86 ()
{
  printf (">>>>>>>> INTERRUPT 86 ! \n");
  outb (0x20, 86);
  outb (0xa0, 86);
}
void __attribute__((optimize("O0"))) interrupt_handler_87 ()
{
  printf (">>>>>>>> INTERRUPT 87 ! \n");
  outb (0x20, 87);
  outb (0xa0, 87);
}
void __attribute__((optimize("O0"))) interrupt_handler_88 ()
{
  printf (">>>>>>>> INTERRUPT 88 ! \n");
  outb (0x20, 88);
  outb (0xa0, 88);
}
void __attribute__((optimize("O0"))) interrupt_handler_89 ()
{
  printf (">>>>>>>> INTERRUPT 89 ! \n");
  outb (0x20, 89);
  outb (0xa0, 89);
}
void __attribute__((optimize("O0"))) interrupt_handler_90 ()
{
  printf (">>>>>>>> INTERRUPT 90 ! \n");
  outb (0x20, 90);
  outb (0xa0, 90);
}
void __attribute__((optimize("O0"))) interrupt_handler_91 ()
{
  printf (">>>>>>>> INTERRUPT 91 ! \n");
  outb (0x20, 91);
  outb (0xa0, 91);
}
void __attribute__((optimize("O0"))) interrupt_handler_92 ()
{
  printf (">>>>>>>> INTERRUPT 92 ! \n");
  outb (0x20, 92);
  outb (0xa0, 92);
}
void __attribute__((optimize("O0"))) interrupt_handler_93 ()
{
  printf (">>>>>>>> INTERRUPT 93 ! \n");
  outb (0x20, 93);
  outb (0xa0, 93);
}
void __attribute__((optimize("O0"))) interrupt_handler_94 ()
{
  printf (">>>>>>>> INTERRUPT 94 ! \n");
  outb (0x20, 94);
  outb (0xa0, 94);
}
void __attribute__((optimize("O0"))) interrupt_handler_95 ()
{
  printf (">>>>>>>> INTERRUPT 95 ! \n");
  outb (0x20, 95);
  outb (0xa0, 95);
}
void __attribute__((optimize("O0"))) interrupt_handler_96 ()
{
  printf (">>>>>>>> INTERRUPT 96 ! \n");
  outb (0x20, 96);
  outb (0xa0, 96);
}
void __attribute__((optimize("O0"))) interrupt_handler_97 ()
{
  printf (">>>>>>>> INTERRUPT 97 ! \n");
  outb (0x20, 97);
  outb (0xa0, 97);
}
void __attribute__((optimize("O0"))) interrupt_handler_98 ()
{
  printf (">>>>>>>> INTERRUPT 98 ! \n");
  outb (0x20, 98);
  outb (0xa0, 98);
}
void __attribute__((optimize("O0"))) interrupt_handler_99 ()
{
  printf (">>>>>>>> INTERRUPT 99 ! \n");
  outb (0x20, 99);
  outb (0xa0, 99);
}
void __attribute__((optimize("O0"))) interrupt_handler_100 ()
{
  printf (">>>>>>>> INTERRUPT 100 ! \n");
  outb (0x20, 100);
  outb (0xa0, 100);
}
void __attribute__((optimize("O0"))) interrupt_handler_101 ()
{
  printf (">>>>>>>> INTERRUPT 101 ! \n");
  outb (0x20, 101);
  outb (0xa0, 101);
}
void __attribute__((optimize("O0"))) interrupt_handler_102 ()
{
  printf (">>>>>>>> INTERRUPT 102 ! \n");
  outb (0x20, 102);
  outb (0xa0, 102);
}
void __attribute__((optimize("O0"))) interrupt_handler_103 ()
{
  printf (">>>>>>>> INTERRUPT 103 ! \n");
  outb (0x20, 103);
  outb (0xa0, 103);
}
void __attribute__((optimize("O0"))) interrupt_handler_104 ()
{
  printf (">>>>>>>> INTERRUPT 104 ! \n");
  outb (0x20, 104);
  outb (0xa0, 104);
}
void __attribute__((optimize("O0"))) interrupt_handler_105 ()
{
  printf (">>>>>>>> INTERRUPT 105 ! \n");
  outb (0x20, 105);
  outb (0xa0, 105);
}
void __attribute__((optimize("O0"))) interrupt_handler_106 ()
{
  printf (">>>>>>>> INTERRUPT 106 ! \n");
  outb (0x20, 106);
  outb (0xa0, 106);
}
void __attribute__((optimize("O0"))) interrupt_handler_107 ()
{
  printf (">>>>>>>> INTERRUPT 107 ! \n");
  outb (0x20, 107);
  outb (0xa0, 107);
}
void __attribute__((optimize("O0"))) interrupt_handler_108 ()
{
  printf (">>>>>>>> INTERRUPT 108 ! \n");
  outb (0x20, 108);
  outb (0xa0, 108);
}
void __attribute__((optimize("O0"))) interrupt_handler_109 ()
{
  printf (">>>>>>>> INTERRUPT 109 ! \n");
  outb (0x20, 109);
  outb (0xa0, 109);
}
void __attribute__((optimize("O0"))) interrupt_handler_110 ()
{
  printf (">>>>>>>> INTERRUPT 110 ! \n");
  outb (0x20, 110);
  outb (0xa0, 110);
}
void __attribute__((optimize("O0"))) interrupt_handler_111 ()
{
  printf (">>>>>>>> INTERRUPT 111 ! \n");
  outb (0x20, 111);
  outb (0xa0, 111);
}
void __attribute__((optimize("O0"))) interrupt_handler_112 ()
{
  printf (">>>>>>>> INTERRUPT 112 ! \n");
  outb (0x20, 112);
  outb (0xa0, 112);
}
void __attribute__((optimize("O0"))) interrupt_handler_113 ()
{
  printf (">>>>>>>> INTERRUPT 113 ! \n");
  outb (0x20, 113);
  outb (0xa0, 113);
}
void __attribute__((optimize("O0"))) interrupt_handler_114 ()
{
  printf (">>>>>>>> INTERRUPT 114 ! \n");
  outb (0x20, 114);
  outb (0xa0, 114);
}
void __attribute__((optimize("O0"))) interrupt_handler_115 ()
{
  printf (">>>>>>>> INTERRUPT 115 ! \n");
  outb (0x20, 115);
  outb (0xa0, 115);
}
void __attribute__((optimize("O0"))) interrupt_handler_116 ()
{
  printf (">>>>>>>> INTERRUPT 116 ! \n");
  outb (0x20, 116);
  outb (0xa0, 116);
}
void __attribute__((optimize("O0"))) interrupt_handler_117 ()
{
  printf (">>>>>>>> INTERRUPT 117 ! \n");
  outb (0x20, 117);
  outb (0xa0, 117);
}
void __attribute__((optimize("O0"))) interrupt_handler_118 ()
{
  printf (">>>>>>>> INTERRUPT 118 ! \n");
  outb (0x20, 118);
  outb (0xa0, 118);
}
void __attribute__((optimize("O0"))) interrupt_handler_119 ()
{
  printf (">>>>>>>> INTERRUPT 119 ! \n");
  outb (0x20, 119);
  outb (0xa0, 119);
}
void __attribute__((optimize("O0"))) interrupt_handler_120 ()
{
  printf (">>>>>>>> INTERRUPT 120 ! \n");
  outb (0x20, 120);
  outb (0xa0, 120);
}
void __attribute__((optimize("O0"))) interrupt_handler_121 ()
{
  printf (">>>>>>>> INTERRUPT 121 ! \n");
  outb (0x20, 121);
  outb (0xa0, 121);
}
void __attribute__((optimize("O0"))) interrupt_handler_122 ()
{
  printf (">>>>>>>> INTERRUPT 122 ! \n");
  outb (0x20, 122);
  outb (0xa0, 122);
}
void __attribute__((optimize("O0"))) interrupt_handler_123 ()
{
  printf (">>>>>>>> INTERRUPT 123 ! \n");
  outb (0x20, 123);
  outb (0xa0, 123);
}
void __attribute__((optimize("O0"))) interrupt_handler_124 ()
{
  printf (">>>>>>>> INTERRUPT 124 ! \n");
  outb (0x20, 124);
  outb (0xa0, 124);
}
void __attribute__((optimize("O0"))) interrupt_handler_125 ()
{
  printf (">>>>>>>> INTERRUPT 125 ! \n");
  outb (0x20, 125);
  outb (0xa0, 125);
}
void __attribute__((optimize("O0"))) interrupt_handler_126 ()
{
  printf (">>>>>>>> INTERRUPT 126 ! \n");
  outb (0x20, 126);
  outb (0xa0, 126);
}
void __attribute__((optimize("O0"))) interrupt_handler_127 ()
{
  printf (">>>>>>>> INTERRUPT 127 ! \n");
  outb (0x20, 127);
  outb (0xa0, 127);
}
void __attribute__((optimize("O0"))) interrupt_handler_128 ()
{
  printf (">>>>>>>> INTERRUPT 128 ! \n");
  outb (0x20, 128);
  outb (0xa0, 128);
}
void __attribute__((optimize("O0"))) interrupt_handler_129 ()
{
  printf (">>>>>>>> INTERRUPT 129 ! \n");
  outb (0x20, 129);
  outb (0xa0, 129);
}
void __attribute__((optimize("O0"))) interrupt_handler_130 ()
{
  printf (">>>>>>>> INTERRUPT 130 ! \n");
  outb (0x20, 130);
  outb (0xa0, 130);
}
void __attribute__((optimize("O0"))) interrupt_handler_131 ()
{
  printf (">>>>>>>> INTERRUPT 131 ! \n");
  outb (0x20, 131);
  outb (0xa0, 131);
}
void __attribute__((optimize("O0"))) interrupt_handler_132 ()
{
  printf (">>>>>>>> INTERRUPT 132 ! \n");
  outb (0x20, 132);
  outb (0xa0, 132);
}
void __attribute__((optimize("O0"))) interrupt_handler_133 ()
{
  printf (">>>>>>>> INTERRUPT 133 ! \n");
  outb (0x20, 133);
  outb (0xa0, 133);
}
void __attribute__((optimize("O0"))) interrupt_handler_134 ()
{
  printf (">>>>>>>> INTERRUPT 134 ! \n");
  outb (0x20, 134);
  outb (0xa0, 134);
}
void __attribute__((optimize("O0"))) interrupt_handler_135 ()
{
  printf (">>>>>>>> INTERRUPT 135 ! \n");
  outb (0x20, 135);
  outb (0xa0, 135);
}
void __attribute__((optimize("O0"))) interrupt_handler_136 ()
{
  printf (">>>>>>>> INTERRUPT 136 ! \n");
  outb (0x20, 136);
  outb (0xa0, 136);
}
void __attribute__((optimize("O0"))) interrupt_handler_137 ()
{
  printf (">>>>>>>> INTERRUPT 137 ! \n");
  outb (0x20, 137);
  outb (0xa0, 137);
}
void __attribute__((optimize("O0"))) interrupt_handler_138 ()
{
  printf (">>>>>>>> INTERRUPT 138 ! \n");
  outb (0x20, 138);
  outb (0xa0, 138);
}
void __attribute__((optimize("O0"))) interrupt_handler_139 ()
{
  printf (">>>>>>>> INTERRUPT 139 ! \n");
  outb (0x20, 139);
  outb (0xa0, 139);
}
void __attribute__((optimize("O0"))) interrupt_handler_140 ()
{
  printf (">>>>>>>> INTERRUPT 140 ! \n");
  outb (0x20, 140);
  outb (0xa0, 140);
}
void __attribute__((optimize("O0"))) interrupt_handler_141 ()
{
  printf (">>>>>>>> INTERRUPT 141 ! \n");
  outb (0x20, 141);
  outb (0xa0, 141);
}
void __attribute__((optimize("O0"))) interrupt_handler_142 ()
{
  printf (">>>>>>>> INTERRUPT 142 ! \n");
  outb (0x20, 142);
  outb (0xa0, 142);
}
void __attribute__((optimize("O0"))) interrupt_handler_143 ()
{
  printf (">>>>>>>> INTERRUPT 143 ! \n");
  outb (0x20, 143);
  outb (0xa0, 143);
}
void __attribute__((optimize("O0"))) interrupt_handler_144 ()
{
  printf (">>>>>>>> INTERRUPT 144 ! \n");
  outb (0x20, 144);
  outb (0xa0, 144);
}
void __attribute__((optimize("O0"))) interrupt_handler_145 ()
{
  printf (">>>>>>>> INTERRUPT 145 ! \n");
  outb (0x20, 145);
  outb (0xa0, 145);
}
void __attribute__((optimize("O0"))) interrupt_handler_146 ()
{
  printf (">>>>>>>> INTERRUPT 146 ! \n");
  outb (0x20, 146);
  outb (0xa0, 146);
}
void __attribute__((optimize("O0"))) interrupt_handler_147 ()
{
  printf (">>>>>>>> INTERRUPT 147 ! \n");
  outb (0x20, 147);
  outb (0xa0, 147);
}
void __attribute__((optimize("O0"))) interrupt_handler_148 ()
{
  printf (">>>>>>>> INTERRUPT 148 ! \n");
  outb (0x20, 148);
  outb (0xa0, 148);
}
void __attribute__((optimize("O0"))) interrupt_handler_149 ()
{
  printf (">>>>>>>> INTERRUPT 149 ! \n");
  outb (0x20, 149);
  outb (0xa0, 149);
}
void __attribute__((optimize("O0"))) interrupt_handler_150 ()
{
  printf (">>>>>>>> INTERRUPT 150 ! \n");
  outb (0x20, 150);
  outb (0xa0, 150);
}
void __attribute__((optimize("O0"))) interrupt_handler_151 ()
{
  printf (">>>>>>>> INTERRUPT 151 ! \n");
  outb (0x20, 151);
  outb (0xa0, 151);
}
void __attribute__((optimize("O0"))) interrupt_handler_152 ()
{
  printf (">>>>>>>> INTERRUPT 152 ! \n");
  outb (0x20, 152);
  outb (0xa0, 152);
}
void __attribute__((optimize("O0"))) interrupt_handler_153 ()
{
  printf (">>>>>>>> INTERRUPT 153 ! \n");
  outb (0x20, 153);
  outb (0xa0, 153);
}
void __attribute__((optimize("O0"))) interrupt_handler_154 ()
{
  printf (">>>>>>>> INTERRUPT 154 ! \n");
  outb (0x20, 154);
  outb (0xa0, 154);
}
void __attribute__((optimize("O0"))) interrupt_handler_155 ()
{
  printf (">>>>>>>> INTERRUPT 155 ! \n");
  outb (0x20, 155);
  outb (0xa0, 155);
}
void __attribute__((optimize("O0"))) interrupt_handler_156 ()
{
  printf (">>>>>>>> INTERRUPT 156 ! \n");
  outb (0x20, 156);
  outb (0xa0, 156);
}
void __attribute__((optimize("O0"))) interrupt_handler_157 ()
{
  printf (">>>>>>>> INTERRUPT 157 ! \n");
  outb (0x20, 157);
  outb (0xa0, 157);
}
void __attribute__((optimize("O0"))) interrupt_handler_158 ()
{
  printf (">>>>>>>> INTERRUPT 158 ! \n");
  outb (0x20, 158);
  outb (0xa0, 158);
}
void __attribute__((optimize("O0"))) interrupt_handler_159 ()
{
  printf (">>>>>>>> INTERRUPT 159 ! \n");
  outb (0x20, 159);
  outb (0xa0, 159);
}
void __attribute__((optimize("O0"))) interrupt_handler_160 ()
{
  printf (">>>>>>>> INTERRUPT 160 ! \n");
  outb (0x20, 160);
  outb (0xa0, 160);
}
void __attribute__((optimize("O0"))) interrupt_handler_161 ()
{
  printf (">>>>>>>> INTERRUPT 161 ! \n");
  outb (0x20, 161);
  outb (0xa0, 161);
}
void __attribute__((optimize("O0"))) interrupt_handler_162 ()
{
  printf (">>>>>>>> INTERRUPT 162 ! \n");
  outb (0x20, 162);
  outb (0xa0, 162);
}
void __attribute__((optimize("O0"))) interrupt_handler_163 ()
{
  printf (">>>>>>>> INTERRUPT 163 ! \n");
  outb (0x20, 163);
  outb (0xa0, 163);
}
void __attribute__((optimize("O0"))) interrupt_handler_164 ()
{
  printf (">>>>>>>> INTERRUPT 164 ! \n");
  outb (0x20, 164);
  outb (0xa0, 164);
}
void __attribute__((optimize("O0"))) interrupt_handler_165 ()
{
  printf (">>>>>>>> INTERRUPT 165 ! \n");
  outb (0x20, 165);
  outb (0xa0, 165);
}
void __attribute__((optimize("O0"))) interrupt_handler_166 ()
{
  printf (">>>>>>>> INTERRUPT 166 ! \n");
  outb (0x20, 166);
  outb (0xa0, 166);
}
void __attribute__((optimize("O0"))) interrupt_handler_167 ()
{
  printf (">>>>>>>> INTERRUPT 167 ! \n");
  outb (0x20, 167);
  outb (0xa0, 167);
}
void __attribute__((optimize("O0"))) interrupt_handler_168 ()
{
  printf (">>>>>>>> INTERRUPT 168 ! \n");
  outb (0x20, 168);
  outb (0xa0, 168);
}
void __attribute__((optimize("O0"))) interrupt_handler_169 ()
{
  printf (">>>>>>>> INTERRUPT 169 ! \n");
  outb (0x20, 169);
  outb (0xa0, 169);
}
void __attribute__((optimize("O0"))) interrupt_handler_170 ()
{
  printf (">>>>>>>> INTERRUPT 170 ! \n");
  outb (0x20, 170);
  outb (0xa0, 170);
}
void __attribute__((optimize("O0"))) interrupt_handler_171 ()
{
  printf (">>>>>>>> INTERRUPT 171 ! \n");
  outb (0x20, 171);
  outb (0xa0, 171);
}
void __attribute__((optimize("O0"))) interrupt_handler_172 ()
{
  printf (">>>>>>>> INTERRUPT 172 ! \n");
  outb (0x20, 172);
  outb (0xa0, 172);
}
void __attribute__((optimize("O0"))) interrupt_handler_173 ()
{
  printf (">>>>>>>> INTERRUPT 173 ! \n");
  outb (0x20, 173);
  outb (0xa0, 173);
}
void __attribute__((optimize("O0"))) interrupt_handler_174 ()
{
  printf (">>>>>>>> INTERRUPT 174 ! \n");
  outb (0x20, 174);
  outb (0xa0, 174);
}
void __attribute__((optimize("O0"))) interrupt_handler_175 ()
{
  printf (">>>>>>>> INTERRUPT 175 ! \n");
  outb (0x20, 175);
  outb (0xa0, 175);
}
void __attribute__((optimize("O0"))) interrupt_handler_176 ()
{
  printf (">>>>>>>> INTERRUPT 176 ! \n");
  outb (0x20, 176);
  outb (0xa0, 176);
}
void __attribute__((optimize("O0"))) interrupt_handler_177 ()
{
  printf (">>>>>>>> INTERRUPT 177 ! \n");
  outb (0x20, 177);
  outb (0xa0, 177);
}
void __attribute__((optimize("O0"))) interrupt_handler_178 ()
{
  printf (">>>>>>>> INTERRUPT 178 ! \n");
  outb (0x20, 178);
  outb (0xa0, 178);
}
void __attribute__((optimize("O0"))) interrupt_handler_179 ()
{
  printf (">>>>>>>> INTERRUPT 179 ! \n");
  outb (0x20, 179);
  outb (0xa0, 179);
}
void __attribute__((optimize("O0"))) interrupt_handler_180 ()
{
  printf (">>>>>>>> INTERRUPT 180 ! \n");
  outb (0x20, 180);
  outb (0xa0, 180);
}
void __attribute__((optimize("O0"))) interrupt_handler_181 ()
{
  printf (">>>>>>>> INTERRUPT 181 ! \n");
  outb (0x20, 181);
  outb (0xa0, 181);
}
void __attribute__((optimize("O0"))) interrupt_handler_182 ()
{
  printf (">>>>>>>> INTERRUPT 182 ! \n");
  outb (0x20, 182);
  outb (0xa0, 182);
}
void __attribute__((optimize("O0"))) interrupt_handler_183 ()
{
  printf (">>>>>>>> INTERRUPT 183 ! \n");
  outb (0x20, 183);
  outb (0xa0, 183);
}
void __attribute__((optimize("O0"))) interrupt_handler_184 ()
{
  printf (">>>>>>>> INTERRUPT 184 ! \n");
  outb (0x20, 184);
  outb (0xa0, 184);
}
void __attribute__((optimize("O0"))) interrupt_handler_185 ()
{
  printf (">>>>>>>> INTERRUPT 185 ! \n");
  outb (0x20, 185);
  outb (0xa0, 185);
}
void __attribute__((optimize("O0"))) interrupt_handler_186 ()
{
  printf (">>>>>>>> INTERRUPT 186 ! \n");
  outb (0x20, 186);
  outb (0xa0, 186);
}
void __attribute__((optimize("O0"))) interrupt_handler_187 ()
{
  printf (">>>>>>>> INTERRUPT 187 ! \n");
  outb (0x20, 187);
  outb (0xa0, 187);
}
void __attribute__((optimize("O0"))) interrupt_handler_188 ()
{
  printf (">>>>>>>> INTERRUPT 188 ! \n");
  outb (0x20, 188);
  outb (0xa0, 188);
}
void __attribute__((optimize("O0"))) interrupt_handler_189 ()
{
  printf (">>>>>>>> INTERRUPT 189 ! \n");
  outb (0x20, 189);
  outb (0xa0, 189);
}
void __attribute__((optimize("O0"))) interrupt_handler_190 ()
{
  printf (">>>>>>>> INTERRUPT 190 ! \n");
  outb (0x20, 190);
  outb (0xa0, 190);
}
void __attribute__((optimize("O0"))) interrupt_handler_191 ()
{
  printf (">>>>>>>> INTERRUPT 191 ! \n");
  outb (0x20, 191);
  outb (0xa0, 191);
}
void __attribute__((optimize("O0"))) interrupt_handler_192 ()
{
  printf (">>>>>>>> INTERRUPT 192 ! \n");
  outb (0x20, 192);
  outb (0xa0, 192);
}
void __attribute__((optimize("O0"))) interrupt_handler_193 ()
{
  printf (">>>>>>>> INTERRUPT 193 ! \n");
  outb (0x20, 193);
  outb (0xa0, 193);
}
void __attribute__((optimize("O0"))) interrupt_handler_194 ()
{
  printf (">>>>>>>> INTERRUPT 194 ! \n");
  outb (0x20, 194);
  outb (0xa0, 194);
}
void __attribute__((optimize("O0"))) interrupt_handler_195 ()
{
  printf (">>>>>>>> INTERRUPT 195 ! \n");
  outb (0x20, 195);
  outb (0xa0, 195);
}
void __attribute__((optimize("O0"))) interrupt_handler_196 ()
{
  printf (">>>>>>>> INTERRUPT 196 ! \n");
  outb (0x20, 196);
  outb (0xa0, 196);
}
void __attribute__((optimize("O0"))) interrupt_handler_197 ()
{
  printf (">>>>>>>> INTERRUPT 197 ! \n");
  outb (0x20, 197);
  outb (0xa0, 197);
}
void __attribute__((optimize("O0"))) interrupt_handler_198 ()
{
  printf (">>>>>>>> INTERRUPT 198 ! \n");
  outb (0x20, 198);
  outb (0xa0, 198);
}
void __attribute__((optimize("O0"))) interrupt_handler_199 ()
{
  printf (">>>>>>>> INTERRUPT 199 ! \n");
  outb (0x20, 199);
  outb (0xa0, 199);
}
void __attribute__((optimize("O0"))) interrupt_handler_200 ()
{
  printf (">>>>>>>> INTERRUPT 200 ! \n");
  outb (0x20, 200);
  outb (0xa0, 200);
}
void __attribute__((optimize("O0"))) interrupt_handler_201 ()
{
  printf (">>>>>>>> INTERRUPT 201 ! \n");
  outb (0x20, 201);
  outb (0xa0, 201);
}
void __attribute__((optimize("O0"))) interrupt_handler_202 ()
{
  printf (">>>>>>>> INTERRUPT 202 ! \n");
  outb (0x20, 202);
  outb (0xa0, 202);
}
void __attribute__((optimize("O0"))) interrupt_handler_203 ()
{
  printf (">>>>>>>> INTERRUPT 203 ! \n");
  outb (0x20, 203);
  outb (0xa0, 203);
}
void __attribute__((optimize("O0"))) interrupt_handler_204 ()
{
  printf (">>>>>>>> INTERRUPT 204 ! \n");
  outb (0x20, 204);
  outb (0xa0, 204);
}
void __attribute__((optimize("O0"))) interrupt_handler_205 ()
{
  printf (">>>>>>>> INTERRUPT 205 ! \n");
  outb (0x20, 205);
  outb (0xa0, 205);
}
void __attribute__((optimize("O0"))) interrupt_handler_206 ()
{
  printf (">>>>>>>> INTERRUPT 206 ! \n");
  outb (0x20, 206);
  outb (0xa0, 206);
}
void __attribute__((optimize("O0"))) interrupt_handler_207 ()
{
  printf (">>>>>>>> INTERRUPT 207 ! \n");
  outb (0x20, 207);
  outb (0xa0, 207);
}
void __attribute__((optimize("O0"))) interrupt_handler_208 ()
{
  printf (">>>>>>>> INTERRUPT 208 ! \n");
  outb (0x20, 208);
  outb (0xa0, 208);
}
void __attribute__((optimize("O0"))) interrupt_handler_209 ()
{
  printf (">>>>>>>> INTERRUPT 209 ! \n");
  outb (0x20, 209);
  outb (0xa0, 209);
}
void __attribute__((optimize("O0"))) interrupt_handler_210 ()
{
  printf (">>>>>>>> INTERRUPT 210 ! \n");
  outb (0x20, 210);
  outb (0xa0, 210);
}
void __attribute__((optimize("O0"))) interrupt_handler_211 ()
{
  printf (">>>>>>>> INTERRUPT 211 ! \n");
  outb (0x20, 211);
  outb (0xa0, 211);
}
void __attribute__((optimize("O0"))) interrupt_handler_212 ()
{
  printf (">>>>>>>> INTERRUPT 212 ! \n");
  outb (0x20, 212);
  outb (0xa0, 212);
}
void __attribute__((optimize("O0"))) interrupt_handler_213 ()
{
  printf (">>>>>>>> INTERRUPT 213 ! \n");
  outb (0x20, 213);
  outb (0xa0, 213);
}
void __attribute__((optimize("O0"))) interrupt_handler_214 ()
{
  printf (">>>>>>>> INTERRUPT 214 ! \n");
  outb (0x20, 214);
  outb (0xa0, 214);
}
void __attribute__((optimize("O0"))) interrupt_handler_215 ()
{
  printf (">>>>>>>> INTERRUPT 215 ! \n");
  outb (0x20, 215);
  outb (0xa0, 215);
}
void __attribute__((optimize("O0"))) interrupt_handler_216 ()
{
  printf (">>>>>>>> INTERRUPT 216 ! \n");
  outb (0x20, 216);
  outb (0xa0, 216);
}
void __attribute__((optimize("O0"))) interrupt_handler_217 ()
{
  printf (">>>>>>>> INTERRUPT 217 ! \n");
  outb (0x20, 217);
  outb (0xa0, 217);
}
void __attribute__((optimize("O0"))) interrupt_handler_218 ()
{
  printf (">>>>>>>> INTERRUPT 218 ! \n");
  outb (0x20, 218);
  outb (0xa0, 218);
}
void __attribute__((optimize("O0"))) interrupt_handler_219 ()
{
  printf (">>>>>>>> INTERRUPT 219 ! \n");
  outb (0x20, 219);
  outb (0xa0, 219);
}
void __attribute__((optimize("O0"))) interrupt_handler_220 ()
{
  printf (">>>>>>>> INTERRUPT 220 ! \n");
  outb (0x20, 220);
  outb (0xa0, 220);
}
void __attribute__((optimize("O0"))) interrupt_handler_221 ()
{
  printf (">>>>>>>> INTERRUPT 221 ! \n");
  outb (0x20, 221);
  outb (0xa0, 221);
}
void __attribute__((optimize("O0"))) interrupt_handler_222 ()
{
  printf (">>>>>>>> INTERRUPT 222 ! \n");
  outb (0x20, 222);
  outb (0xa0, 222);
}
void __attribute__((optimize("O0"))) interrupt_handler_223 ()
{
  printf (">>>>>>>> INTERRUPT 223 ! \n");
  outb (0x20, 223);
  outb (0xa0, 223);
}
void __attribute__((optimize("O0"))) interrupt_handler_224 ()
{
  printf (">>>>>>>> INTERRUPT 224 ! \n");
  outb (0x20, 224);
  outb (0xa0, 224);
}
void __attribute__((optimize("O0"))) interrupt_handler_225 ()
{
  printf (">>>>>>>> INTERRUPT 225 ! \n");
  outb (0x20, 225);
  outb (0xa0, 225);
}
void __attribute__((optimize("O0"))) interrupt_handler_226 ()
{
  printf (">>>>>>>> INTERRUPT 226 ! \n");
  outb (0x20, 226);
  outb (0xa0, 226);
}
void __attribute__((optimize("O0"))) interrupt_handler_227 ()
{
  printf (">>>>>>>> INTERRUPT 227 ! \n");
  outb (0x20, 227);
  outb (0xa0, 227);
}
void __attribute__((optimize("O0"))) interrupt_handler_228 ()
{
  printf (">>>>>>>> INTERRUPT 228 ! \n");
  outb (0x20, 228);
  outb (0xa0, 228);
}
void __attribute__((optimize("O0"))) interrupt_handler_229 ()
{
  printf (">>>>>>>> INTERRUPT 229 ! \n");
  outb (0x20, 229);
  outb (0xa0, 229);
}
void __attribute__((optimize("O0"))) interrupt_handler_230 ()
{
  printf (">>>>>>>> INTERRUPT 230 ! \n");
  outb (0x20, 230);
  outb (0xa0, 230);
}
void __attribute__((optimize("O0"))) interrupt_handler_231 ()
{
  printf (">>>>>>>> INTERRUPT 231 ! \n");
  outb (0x20, 231);
  outb (0xa0, 231);
}
void __attribute__((optimize("O0"))) interrupt_handler_232 ()
{
  printf (">>>>>>>> INTERRUPT 232 ! \n");
  outb (0x20, 232);
  outb (0xa0, 232);
}
void __attribute__((optimize("O0"))) interrupt_handler_233 ()
{
  printf (">>>>>>>> INTERRUPT 233 ! \n");
  outb (0x20, 233);
  outb (0xa0, 233);
}
void __attribute__((optimize("O0"))) interrupt_handler_234 ()
{
  printf (">>>>>>>> INTERRUPT 234 ! \n");
  outb (0x20, 234);
  outb (0xa0, 234);
}
void __attribute__((optimize("O0"))) interrupt_handler_235 ()
{
  printf (">>>>>>>> INTERRUPT 235 ! \n");
  outb (0x20, 235);
  outb (0xa0, 235);
}
void __attribute__((optimize("O0"))) interrupt_handler_236 ()
{
  printf (">>>>>>>> INTERRUPT 236 ! \n");
  outb (0x20, 236);
  outb (0xa0, 236);
}
void __attribute__((optimize("O0"))) interrupt_handler_237 ()
{
  printf (">>>>>>>> INTERRUPT 237 ! \n");
  outb (0x20, 237);
  outb (0xa0, 237);
}
void __attribute__((optimize("O0"))) interrupt_handler_238 ()
{
  printf (">>>>>>>> INTERRUPT 238 ! \n");
  outb (0x20, 238);
  outb (0xa0, 238);
}
void __attribute__((optimize("O0"))) interrupt_handler_239 ()
{
  printf (">>>>>>>> INTERRUPT 239 ! \n");
  outb (0x20, 239);
  outb (0xa0, 239);
}
void __attribute__((optimize("O0"))) interrupt_handler_240 ()
{
  printf (">>>>>>>> INTERRUPT 240 ! \n");
  outb (0x20, 240);
  outb (0xa0, 240);
}
void __attribute__((optimize("O0"))) interrupt_handler_241 ()
{
  printf (">>>>>>>> INTERRUPT 241 ! \n");
  outb (0x20, 241);
  outb (0xa0, 241);
}
void __attribute__((optimize("O0"))) interrupt_handler_242 ()
{
  printf (">>>>>>>> INTERRUPT 242 ! \n");
  outb (0x20, 242);
  outb (0xa0, 242);
}
void __attribute__((optimize("O0"))) interrupt_handler_243 ()
{
  printf (">>>>>>>> INTERRUPT 243 ! \n");
  outb (0x20, 243);
  outb (0xa0, 243);
}
void __attribute__((optimize("O0"))) interrupt_handler_244 ()
{
  printf (">>>>>>>> INTERRUPT 244 ! \n");
  outb (0x20, 244);
  outb (0xa0, 244);
}
void __attribute__((optimize("O0"))) interrupt_handler_245 ()
{
  printf (">>>>>>>> INTERRUPT 245 ! \n");
  outb (0x20, 245);
  outb (0xa0, 245);
}
void __attribute__((optimize("O0"))) interrupt_handler_246 ()
{
  printf (">>>>>>>> INTERRUPT 246 ! \n");
  outb (0x20, 246);
  outb (0xa0, 246);
}
void __attribute__((optimize("O0"))) interrupt_handler_247 ()
{
  printf (">>>>>>>> INTERRUPT 247 ! \n");
  outb (0x20, 247);
  outb (0xa0, 247);
}
void __attribute__((optimize("O0"))) interrupt_handler_248 ()
{
  printf (">>>>>>>> INTERRUPT 248 ! \n");
  outb (0x20, 248);
  outb (0xa0, 248);
}
void __attribute__((optimize("O0"))) interrupt_handler_249 ()
{
  printf (">>>>>>>> INTERRUPT 249 ! \n");
  outb (0x20, 249);
  outb (0xa0, 249);
}
void __attribute__((optimize("O0"))) interrupt_handler_250 ()
{
  printf (">>>>>>>> INTERRUPT 250 ! \n");
  outb (0x20, 250);
  outb (0xa0, 250);
}
void __attribute__((optimize("O0"))) interrupt_handler_251 ()
{
  printf (">>>>>>>> INTERRUPT 251 ! \n");
  outb (0x20, 251);
  outb (0xa0, 251);
}
void __attribute__((optimize("O0"))) interrupt_handler_252 ()
{
  printf (">>>>>>>> INTERRUPT 252 ! \n");
  outb (0x20, 252);
  outb (0xa0, 252);
}
void __attribute__((optimize("O0"))) interrupt_handler_253 ()
{
  printf (">>>>>>>> INTERRUPT 253 ! \n");
  outb (0x20, 253);
  outb (0xa0, 253);
}
void __attribute__((optimize("O0"))) interrupt_handler_254 ()
{
  printf (">>>>>>>> INTERRUPT 254 ! \n");
  outb (0x20, 254);
  outb (0xa0, 254);
}
void __attribute__((optimize("O0"))) interrupt_handler_255 ()
{
  printf (">>>>>>>> INTERRUPT 255 ! \n");
  outb (0x20, 255);
  outb (0xa0, 255);
}
