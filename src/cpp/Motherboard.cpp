//
// Created by Yan Pujante on 7/15/15.
//

#include "Motherboard.h"

void MinMaxAutoResetJBoxProperty::onPropertyUpdated(const TJBox_Int32 &, const TJBox_Int32 &autoReset)
{
  if(autoReset == 0)
    fAutoResetFrameCount = MIN_MAX_AUTO_RESET_OFF;
  else
    fAutoResetFrameCount = batchCountToFitDuration(fSampleRate, autoReset * 1000);
}

Motherboard::Motherboard(int iSampleRate):
  fCVIn1("/cv_inputs/cv_in_1"),
  fCVOut1("/cv_outputs/cv_out_1", "/custom_properties/prop_cv_out_1_type"),
  fCVOut2("/cv_outputs/cv_out_2", "/custom_properties/prop_cv_out_2_type"),
  fCVOut3("/cv_outputs/cv_out_3", "/custom_properties/prop_cv_out_3_type"),
  fCVOut4("/cv_outputs/cv_out_4", "/custom_properties/prop_cv_out_4_type"),
  fCVOutValue1("/cv_outputs/cv_out_value_1"),
  fCVOutMinValue1("/cv_outputs/cv_out_min_value_1"),
  fCVOutMaxValue1("/cv_outputs/cv_out_max_value_1"),
  fPropCVIn1Value("/custom_properties/prop_cv_in_1_value_int", "/custom_properties/prop_cv_in_1_value"),
  fPropCVIn1State("/custom_properties/prop_cv_in_1_state"),
  fPropCVIn1ValueAsNote("/custom_properties/prop_cv_in_1_value_as_note"),
  fPropCVIn1MinValue("/custom_properties/prop_cv_in_1_min_value_int", "/custom_properties/prop_cv_in_1_min_value"),
  fPropCVIn1MaxValue("/custom_properties/prop_cv_in_1_max_value_int", "/custom_properties/prop_cv_in_1_max_value"),
  fPropCVIn1MinMaxReset("/custom_properties/prop_cv_in_1_min_max_reset"),
  fPropInputPaused("/custom_properties/prop_input_paused"),
  fPropInputPageOffset("/custom_properties/prop_input_page_offset"),
  fPropInputHistoryOffset("/custom_properties/prop_input_history_offset"),
  fPropInputHistoryOffsetRT("/custom_properties/prop_input_history_offset_rt"),
  fPropZoomFactorX("/custom_properties/prop_zoom_factor_x"),
  fPropZoomFactorY("/custom_properties/prop_zoom_factor_y"),
  fPropPlus05Axis("/custom_properties/prop_plus_05_axis"),
  fPropCVIn1MinMaxAutoReset("/custom_properties/prop_cv_in_1_min_max_auto_reset", iSampleRate),
  fPropScreenOn("/custom_properties/prop_screen_on"),
  fPropMidiOn("/custom_properties/prop_midi_on"),
  fPropArrayStart("/custom_properties/prop_array_start"),
  fPropArray(new Int32JBoxProperty *[MAX_ARRAY_SIZE])
{
  initPropArray();
}

void Motherboard::update(Motherboard const &rhs)
{
  fCVIn1 = rhs.fCVIn1;
  fCVOut1 = rhs.fCVOut1;
  fCVOut2 = rhs.fCVOut2;
  fCVOut3 = rhs.fCVOut3;
  fCVOut4 = rhs.fCVOut4;
  fCVOutMinValue1 = rhs.fCVOutMinValue1;
  fCVOutMaxValue1 = rhs.fCVOutMaxValue1;
  fPropCVIn1MinMaxReset = rhs.fPropCVIn1MinMaxReset;
  fPropInputPaused = rhs.fPropInputPaused;
  fPropInputPageOffset = rhs.fPropInputPageOffset;
  fPropInputHistoryOffset = rhs.fPropInputHistoryOffset;
  fPropZoomFactorX = rhs.fPropZoomFactorX;
  fPropZoomFactorY = rhs.fPropZoomFactorY;
  fPropCVIn1MinMaxAutoReset = rhs.fPropCVIn1MinMaxAutoReset;
  fPropScreenOn = rhs.fPropScreenOn;
  fPropMidiOn = rhs.fPropMidiOn;
}

void Motherboard::registerForUpdate(IJBoxPropertyManager &manager)
{
  fPropCVIn1Value.registerForInit(manager);
  fPropCVIn1MinValue.registerForInit(manager);
  fPropCVIn1MaxValue.registerForInit(manager);
  fPropCVIn1ValueAsNote.registerForInit(manager);
  fPropCVIn1State.registerForInit(manager);
  fPropInputHistoryOffsetRT.registerForInit(manager);

  fCVIn1.registerForUpdate(manager);
  fCVOut1.registerForUpdate(manager, PROP_CV_OUT_1_TYPE_TAG);
  fCVOut2.registerForUpdate(manager, PROP_CV_OUT_2_TYPE_TAG);
  fCVOut3.registerForUpdate(manager, PROP_CV_OUT_3_TYPE_TAG);
  fCVOut4.registerForUpdate(manager, PROP_CV_OUT_4_TYPE_TAG);
  fCVOutValue1.registerForUpdate(manager);
  fCVOutMinValue1.registerForUpdate(manager);
  fCVOutMaxValue1.registerForUpdate(manager);
  fPropCVIn1MinMaxReset.registerForUpdate(manager, PROP_CV_IN_1_MIN_MAX_RESET_TAG);
  fPropInputPaused.registerForUpdate(manager, PROP_INPUT_PAUSED_TAG);
  fPropInputPageOffset.registerForUpdate(manager, PROP_INPUT_PAGE_OFFSET_TAG);
  fPropInputHistoryOffset.registerForUpdate(manager, PROP_INPUT_HISTORY_OFFSET_TAG);
  fPropZoomFactorX.registerForUpdate(manager, PROP_ZOOM_FACTOR_X_TAG);
  fPropZoomFactorY.registerForUpdate(manager, PROP_ZOOM_FACTOR_Y_TAG);
  fPropCVIn1MinMaxAutoReset.registerForUpdate(manager, PROP_CV_IN_1_MIN_MAX_AUTO_RESET_TAG);
  fPropScreenOn.registerForUpdate(manager, PROP_SCREEN_ON_TAG);
  fPropMidiOn.registerForUpdate(manager, PROP_MIDI_ON_TAG);
}

void Motherboard::initPropArray()
{
  fPropArray[0] = new Int32JBoxProperty("/custom_properties/prop_array_1");
  fPropArray[1] = new Int32JBoxProperty("/custom_properties/prop_array_2");
  fPropArray[2] = new Int32JBoxProperty("/custom_properties/prop_array_3");
  fPropArray[3] = new Int32JBoxProperty("/custom_properties/prop_array_4");
  fPropArray[4] = new Int32JBoxProperty("/custom_properties/prop_array_5");
  fPropArray[5] = new Int32JBoxProperty("/custom_properties/prop_array_6");
  fPropArray[6] = new Int32JBoxProperty("/custom_properties/prop_array_7");
  fPropArray[7] = new Int32JBoxProperty("/custom_properties/prop_array_8");
  fPropArray[8] = new Int32JBoxProperty("/custom_properties/prop_array_9");
  fPropArray[9] = new Int32JBoxProperty("/custom_properties/prop_array_10");
  fPropArray[10] = new Int32JBoxProperty("/custom_properties/prop_array_11");
  fPropArray[11] = new Int32JBoxProperty("/custom_properties/prop_array_12");
  fPropArray[12] = new Int32JBoxProperty("/custom_properties/prop_array_13");
  fPropArray[13] = new Int32JBoxProperty("/custom_properties/prop_array_14");
  fPropArray[14] = new Int32JBoxProperty("/custom_properties/prop_array_15");
  fPropArray[15] = new Int32JBoxProperty("/custom_properties/prop_array_16");
  fPropArray[16] = new Int32JBoxProperty("/custom_properties/prop_array_17");
  fPropArray[17] = new Int32JBoxProperty("/custom_properties/prop_array_18");
  fPropArray[18] = new Int32JBoxProperty("/custom_properties/prop_array_19");
  fPropArray[19] = new Int32JBoxProperty("/custom_properties/prop_array_20");
  fPropArray[20] = new Int32JBoxProperty("/custom_properties/prop_array_21");
  fPropArray[21] = new Int32JBoxProperty("/custom_properties/prop_array_22");
  fPropArray[22] = new Int32JBoxProperty("/custom_properties/prop_array_23");
  fPropArray[23] = new Int32JBoxProperty("/custom_properties/prop_array_24");
  fPropArray[24] = new Int32JBoxProperty("/custom_properties/prop_array_25");
  fPropArray[25] = new Int32JBoxProperty("/custom_properties/prop_array_26");
  fPropArray[26] = new Int32JBoxProperty("/custom_properties/prop_array_27");
  fPropArray[27] = new Int32JBoxProperty("/custom_properties/prop_array_28");
  fPropArray[28] = new Int32JBoxProperty("/custom_properties/prop_array_29");
  fPropArray[29] = new Int32JBoxProperty("/custom_properties/prop_array_30");
  fPropArray[30] = new Int32JBoxProperty("/custom_properties/prop_array_31");
  fPropArray[31] = new Int32JBoxProperty("/custom_properties/prop_array_32");
  fPropArray[32] = new Int32JBoxProperty("/custom_properties/prop_array_33");
  fPropArray[33] = new Int32JBoxProperty("/custom_properties/prop_array_34");
  fPropArray[34] = new Int32JBoxProperty("/custom_properties/prop_array_35");
  fPropArray[35] = new Int32JBoxProperty("/custom_properties/prop_array_36");
  fPropArray[36] = new Int32JBoxProperty("/custom_properties/prop_array_37");
  fPropArray[37] = new Int32JBoxProperty("/custom_properties/prop_array_38");
  fPropArray[38] = new Int32JBoxProperty("/custom_properties/prop_array_39");
  fPropArray[39] = new Int32JBoxProperty("/custom_properties/prop_array_40");
  fPropArray[40] = new Int32JBoxProperty("/custom_properties/prop_array_41");
  fPropArray[41] = new Int32JBoxProperty("/custom_properties/prop_array_42");
  fPropArray[42] = new Int32JBoxProperty("/custom_properties/prop_array_43");
  fPropArray[43] = new Int32JBoxProperty("/custom_properties/prop_array_44");
  fPropArray[44] = new Int32JBoxProperty("/custom_properties/prop_array_45");
  fPropArray[45] = new Int32JBoxProperty("/custom_properties/prop_array_46");
  fPropArray[46] = new Int32JBoxProperty("/custom_properties/prop_array_47");
  fPropArray[47] = new Int32JBoxProperty("/custom_properties/prop_array_48");
  fPropArray[48] = new Int32JBoxProperty("/custom_properties/prop_array_49");
  fPropArray[49] = new Int32JBoxProperty("/custom_properties/prop_array_50");
  fPropArray[50] = new Int32JBoxProperty("/custom_properties/prop_array_51");
  fPropArray[51] = new Int32JBoxProperty("/custom_properties/prop_array_52");
  fPropArray[52] = new Int32JBoxProperty("/custom_properties/prop_array_53");
  fPropArray[53] = new Int32JBoxProperty("/custom_properties/prop_array_54");
  fPropArray[54] = new Int32JBoxProperty("/custom_properties/prop_array_55");
  fPropArray[55] = new Int32JBoxProperty("/custom_properties/prop_array_56");
  fPropArray[56] = new Int32JBoxProperty("/custom_properties/prop_array_57");
  fPropArray[57] = new Int32JBoxProperty("/custom_properties/prop_array_58");
  fPropArray[58] = new Int32JBoxProperty("/custom_properties/prop_array_59");
  fPropArray[59] = new Int32JBoxProperty("/custom_properties/prop_array_60");
  fPropArray[60] = new Int32JBoxProperty("/custom_properties/prop_array_61");
  fPropArray[61] = new Int32JBoxProperty("/custom_properties/prop_array_62");
  fPropArray[62] = new Int32JBoxProperty("/custom_properties/prop_array_63");
  fPropArray[63] = new Int32JBoxProperty("/custom_properties/prop_array_64");
  fPropArray[64] = new Int32JBoxProperty("/custom_properties/prop_array_65");
  fPropArray[65] = new Int32JBoxProperty("/custom_properties/prop_array_66");
  fPropArray[66] = new Int32JBoxProperty("/custom_properties/prop_array_67");
  fPropArray[67] = new Int32JBoxProperty("/custom_properties/prop_array_68");
  fPropArray[68] = new Int32JBoxProperty("/custom_properties/prop_array_69");
  fPropArray[69] = new Int32JBoxProperty("/custom_properties/prop_array_70");
  fPropArray[70] = new Int32JBoxProperty("/custom_properties/prop_array_71");
  fPropArray[71] = new Int32JBoxProperty("/custom_properties/prop_array_72");
  fPropArray[72] = new Int32JBoxProperty("/custom_properties/prop_array_73");
  fPropArray[73] = new Int32JBoxProperty("/custom_properties/prop_array_74");
  fPropArray[74] = new Int32JBoxProperty("/custom_properties/prop_array_75");
  fPropArray[75] = new Int32JBoxProperty("/custom_properties/prop_array_76");
  fPropArray[76] = new Int32JBoxProperty("/custom_properties/prop_array_77");
  fPropArray[77] = new Int32JBoxProperty("/custom_properties/prop_array_78");
  fPropArray[78] = new Int32JBoxProperty("/custom_properties/prop_array_79");
  fPropArray[79] = new Int32JBoxProperty("/custom_properties/prop_array_80");
  fPropArray[80] = new Int32JBoxProperty("/custom_properties/prop_array_81");
  fPropArray[81] = new Int32JBoxProperty("/custom_properties/prop_array_82");
  fPropArray[82] = new Int32JBoxProperty("/custom_properties/prop_array_83");
  fPropArray[83] = new Int32JBoxProperty("/custom_properties/prop_array_84");
  fPropArray[84] = new Int32JBoxProperty("/custom_properties/prop_array_85");
  fPropArray[85] = new Int32JBoxProperty("/custom_properties/prop_array_86");
  fPropArray[86] = new Int32JBoxProperty("/custom_properties/prop_array_87");
  fPropArray[87] = new Int32JBoxProperty("/custom_properties/prop_array_88");
  fPropArray[88] = new Int32JBoxProperty("/custom_properties/prop_array_89");
  fPropArray[89] = new Int32JBoxProperty("/custom_properties/prop_array_90");
  fPropArray[90] = new Int32JBoxProperty("/custom_properties/prop_array_91");
  fPropArray[91] = new Int32JBoxProperty("/custom_properties/prop_array_92");
  fPropArray[92] = new Int32JBoxProperty("/custom_properties/prop_array_93");
  fPropArray[93] = new Int32JBoxProperty("/custom_properties/prop_array_94");
  fPropArray[94] = new Int32JBoxProperty("/custom_properties/prop_array_95");
  fPropArray[95] = new Int32JBoxProperty("/custom_properties/prop_array_96");
  fPropArray[96] = new Int32JBoxProperty("/custom_properties/prop_array_97");
  fPropArray[97] = new Int32JBoxProperty("/custom_properties/prop_array_98");
  fPropArray[98] = new Int32JBoxProperty("/custom_properties/prop_array_99");
  fPropArray[99] = new Int32JBoxProperty("/custom_properties/prop_array_100");
  fPropArray[100] = new Int32JBoxProperty("/custom_properties/prop_array_101");
  fPropArray[101] = new Int32JBoxProperty("/custom_properties/prop_array_102");
  fPropArray[102] = new Int32JBoxProperty("/custom_properties/prop_array_103");
  fPropArray[103] = new Int32JBoxProperty("/custom_properties/prop_array_104");
  fPropArray[104] = new Int32JBoxProperty("/custom_properties/prop_array_105");
  fPropArray[105] = new Int32JBoxProperty("/custom_properties/prop_array_106");
  fPropArray[106] = new Int32JBoxProperty("/custom_properties/prop_array_107");
  fPropArray[107] = new Int32JBoxProperty("/custom_properties/prop_array_108");
  fPropArray[108] = new Int32JBoxProperty("/custom_properties/prop_array_109");
  fPropArray[109] = new Int32JBoxProperty("/custom_properties/prop_array_110");
  fPropArray[110] = new Int32JBoxProperty("/custom_properties/prop_array_111");
  fPropArray[111] = new Int32JBoxProperty("/custom_properties/prop_array_112");
  fPropArray[112] = new Int32JBoxProperty("/custom_properties/prop_array_113");
  fPropArray[113] = new Int32JBoxProperty("/custom_properties/prop_array_114");
  fPropArray[114] = new Int32JBoxProperty("/custom_properties/prop_array_115");
  fPropArray[115] = new Int32JBoxProperty("/custom_properties/prop_array_116");
  fPropArray[116] = new Int32JBoxProperty("/custom_properties/prop_array_117");
  fPropArray[117] = new Int32JBoxProperty("/custom_properties/prop_array_118");
  fPropArray[118] = new Int32JBoxProperty("/custom_properties/prop_array_119");
  fPropArray[119] = new Int32JBoxProperty("/custom_properties/prop_array_120");
  fPropArray[120] = new Int32JBoxProperty("/custom_properties/prop_array_121");
  fPropArray[121] = new Int32JBoxProperty("/custom_properties/prop_array_122");
  fPropArray[122] = new Int32JBoxProperty("/custom_properties/prop_array_123");
  fPropArray[123] = new Int32JBoxProperty("/custom_properties/prop_array_124");
  fPropArray[124] = new Int32JBoxProperty("/custom_properties/prop_array_125");
  fPropArray[125] = new Int32JBoxProperty("/custom_properties/prop_array_126");
  fPropArray[126] = new Int32JBoxProperty("/custom_properties/prop_array_127");
  fPropArray[127] = new Int32JBoxProperty("/custom_properties/prop_array_128");
  fPropArray[128] = new Int32JBoxProperty("/custom_properties/prop_array_129");
  fPropArray[129] = new Int32JBoxProperty("/custom_properties/prop_array_130");
  fPropArray[130] = new Int32JBoxProperty("/custom_properties/prop_array_131");
  fPropArray[131] = new Int32JBoxProperty("/custom_properties/prop_array_132");
  fPropArray[132] = new Int32JBoxProperty("/custom_properties/prop_array_133");
  fPropArray[133] = new Int32JBoxProperty("/custom_properties/prop_array_134");
  fPropArray[134] = new Int32JBoxProperty("/custom_properties/prop_array_135");
  fPropArray[135] = new Int32JBoxProperty("/custom_properties/prop_array_136");
  fPropArray[136] = new Int32JBoxProperty("/custom_properties/prop_array_137");
  fPropArray[137] = new Int32JBoxProperty("/custom_properties/prop_array_138");
  fPropArray[138] = new Int32JBoxProperty("/custom_properties/prop_array_139");
  fPropArray[139] = new Int32JBoxProperty("/custom_properties/prop_array_140");
  fPropArray[140] = new Int32JBoxProperty("/custom_properties/prop_array_141");
  fPropArray[141] = new Int32JBoxProperty("/custom_properties/prop_array_142");
  fPropArray[142] = new Int32JBoxProperty("/custom_properties/prop_array_143");
  fPropArray[143] = new Int32JBoxProperty("/custom_properties/prop_array_144");
  fPropArray[144] = new Int32JBoxProperty("/custom_properties/prop_array_145");
  fPropArray[145] = new Int32JBoxProperty("/custom_properties/prop_array_146");
  fPropArray[146] = new Int32JBoxProperty("/custom_properties/prop_array_147");
  fPropArray[147] = new Int32JBoxProperty("/custom_properties/prop_array_148");
  fPropArray[148] = new Int32JBoxProperty("/custom_properties/prop_array_149");
  fPropArray[149] = new Int32JBoxProperty("/custom_properties/prop_array_150");
  fPropArray[150] = new Int32JBoxProperty("/custom_properties/prop_array_151");
  fPropArray[151] = new Int32JBoxProperty("/custom_properties/prop_array_152");
  fPropArray[152] = new Int32JBoxProperty("/custom_properties/prop_array_153");
  fPropArray[153] = new Int32JBoxProperty("/custom_properties/prop_array_154");
  fPropArray[154] = new Int32JBoxProperty("/custom_properties/prop_array_155");
  fPropArray[155] = new Int32JBoxProperty("/custom_properties/prop_array_156");
  fPropArray[156] = new Int32JBoxProperty("/custom_properties/prop_array_157");
  fPropArray[157] = new Int32JBoxProperty("/custom_properties/prop_array_158");
  fPropArray[158] = new Int32JBoxProperty("/custom_properties/prop_array_159");
  fPropArray[159] = new Int32JBoxProperty("/custom_properties/prop_array_160");
  fPropArray[160] = new Int32JBoxProperty("/custom_properties/prop_array_161");
  fPropArray[161] = new Int32JBoxProperty("/custom_properties/prop_array_162");
  fPropArray[162] = new Int32JBoxProperty("/custom_properties/prop_array_163");
  fPropArray[163] = new Int32JBoxProperty("/custom_properties/prop_array_164");
  fPropArray[164] = new Int32JBoxProperty("/custom_properties/prop_array_165");
  fPropArray[165] = new Int32JBoxProperty("/custom_properties/prop_array_166");
  fPropArray[166] = new Int32JBoxProperty("/custom_properties/prop_array_167");
  fPropArray[167] = new Int32JBoxProperty("/custom_properties/prop_array_168");
  fPropArray[168] = new Int32JBoxProperty("/custom_properties/prop_array_169");
  fPropArray[169] = new Int32JBoxProperty("/custom_properties/prop_array_170");
  fPropArray[170] = new Int32JBoxProperty("/custom_properties/prop_array_171");
  fPropArray[171] = new Int32JBoxProperty("/custom_properties/prop_array_172");
  fPropArray[172] = new Int32JBoxProperty("/custom_properties/prop_array_173");
  fPropArray[173] = new Int32JBoxProperty("/custom_properties/prop_array_174");
  fPropArray[174] = new Int32JBoxProperty("/custom_properties/prop_array_175");
  fPropArray[175] = new Int32JBoxProperty("/custom_properties/prop_array_176");
  fPropArray[176] = new Int32JBoxProperty("/custom_properties/prop_array_177");
  fPropArray[177] = new Int32JBoxProperty("/custom_properties/prop_array_178");
  fPropArray[178] = new Int32JBoxProperty("/custom_properties/prop_array_179");
  fPropArray[179] = new Int32JBoxProperty("/custom_properties/prop_array_180");
  fPropArray[180] = new Int32JBoxProperty("/custom_properties/prop_array_181");
  fPropArray[181] = new Int32JBoxProperty("/custom_properties/prop_array_182");
  fPropArray[182] = new Int32JBoxProperty("/custom_properties/prop_array_183");
  fPropArray[183] = new Int32JBoxProperty("/custom_properties/prop_array_184");
  fPropArray[184] = new Int32JBoxProperty("/custom_properties/prop_array_185");
  fPropArray[185] = new Int32JBoxProperty("/custom_properties/prop_array_186");
  fPropArray[186] = new Int32JBoxProperty("/custom_properties/prop_array_187");
  fPropArray[187] = new Int32JBoxProperty("/custom_properties/prop_array_188");
  fPropArray[188] = new Int32JBoxProperty("/custom_properties/prop_array_189");
  fPropArray[189] = new Int32JBoxProperty("/custom_properties/prop_array_190");
  fPropArray[190] = new Int32JBoxProperty("/custom_properties/prop_array_191");
  fPropArray[191] = new Int32JBoxProperty("/custom_properties/prop_array_192");
  fPropArray[192] = new Int32JBoxProperty("/custom_properties/prop_array_193");
  fPropArray[193] = new Int32JBoxProperty("/custom_properties/prop_array_194");
  fPropArray[194] = new Int32JBoxProperty("/custom_properties/prop_array_195");
  fPropArray[195] = new Int32JBoxProperty("/custom_properties/prop_array_196");
  fPropArray[196] = new Int32JBoxProperty("/custom_properties/prop_array_197");
  fPropArray[197] = new Int32JBoxProperty("/custom_properties/prop_array_198");
  fPropArray[198] = new Int32JBoxProperty("/custom_properties/prop_array_199");
  fPropArray[199] = new Int32JBoxProperty("/custom_properties/prop_array_200");
  fPropArray[200] = new Int32JBoxProperty("/custom_properties/prop_array_201");
  fPropArray[201] = new Int32JBoxProperty("/custom_properties/prop_array_202");
  fPropArray[202] = new Int32JBoxProperty("/custom_properties/prop_array_203");
  fPropArray[203] = new Int32JBoxProperty("/custom_properties/prop_array_204");
  fPropArray[204] = new Int32JBoxProperty("/custom_properties/prop_array_205");
  fPropArray[205] = new Int32JBoxProperty("/custom_properties/prop_array_206");
  fPropArray[206] = new Int32JBoxProperty("/custom_properties/prop_array_207");
  fPropArray[207] = new Int32JBoxProperty("/custom_properties/prop_array_208");
  fPropArray[208] = new Int32JBoxProperty("/custom_properties/prop_array_209");
  fPropArray[209] = new Int32JBoxProperty("/custom_properties/prop_array_210");
  fPropArray[210] = new Int32JBoxProperty("/custom_properties/prop_array_211");
  fPropArray[211] = new Int32JBoxProperty("/custom_properties/prop_array_212");
  fPropArray[212] = new Int32JBoxProperty("/custom_properties/prop_array_213");
  fPropArray[213] = new Int32JBoxProperty("/custom_properties/prop_array_214");
  fPropArray[214] = new Int32JBoxProperty("/custom_properties/prop_array_215");
  fPropArray[215] = new Int32JBoxProperty("/custom_properties/prop_array_216");
  fPropArray[216] = new Int32JBoxProperty("/custom_properties/prop_array_217");
  fPropArray[217] = new Int32JBoxProperty("/custom_properties/prop_array_218");
  fPropArray[218] = new Int32JBoxProperty("/custom_properties/prop_array_219");
  fPropArray[219] = new Int32JBoxProperty("/custom_properties/prop_array_220");
  fPropArray[220] = new Int32JBoxProperty("/custom_properties/prop_array_221");
  fPropArray[221] = new Int32JBoxProperty("/custom_properties/prop_array_222");
  fPropArray[222] = new Int32JBoxProperty("/custom_properties/prop_array_223");
  fPropArray[223] = new Int32JBoxProperty("/custom_properties/prop_array_224");
  fPropArray[224] = new Int32JBoxProperty("/custom_properties/prop_array_225");
  fPropArray[225] = new Int32JBoxProperty("/custom_properties/prop_array_226");
  fPropArray[226] = new Int32JBoxProperty("/custom_properties/prop_array_227");
  fPropArray[227] = new Int32JBoxProperty("/custom_properties/prop_array_228");
  fPropArray[228] = new Int32JBoxProperty("/custom_properties/prop_array_229");
  fPropArray[229] = new Int32JBoxProperty("/custom_properties/prop_array_230");
  fPropArray[230] = new Int32JBoxProperty("/custom_properties/prop_array_231");
  fPropArray[231] = new Int32JBoxProperty("/custom_properties/prop_array_232");
  fPropArray[232] = new Int32JBoxProperty("/custom_properties/prop_array_233");
  fPropArray[233] = new Int32JBoxProperty("/custom_properties/prop_array_234");
  fPropArray[234] = new Int32JBoxProperty("/custom_properties/prop_array_235");
  fPropArray[235] = new Int32JBoxProperty("/custom_properties/prop_array_236");
  fPropArray[236] = new Int32JBoxProperty("/custom_properties/prop_array_237");
  fPropArray[237] = new Int32JBoxProperty("/custom_properties/prop_array_238");
  fPropArray[238] = new Int32JBoxProperty("/custom_properties/prop_array_239");
  fPropArray[239] = new Int32JBoxProperty("/custom_properties/prop_array_240");
  fPropArray[240] = new Int32JBoxProperty("/custom_properties/prop_array_241");
  fPropArray[241] = new Int32JBoxProperty("/custom_properties/prop_array_242");
  fPropArray[242] = new Int32JBoxProperty("/custom_properties/prop_array_243");
  fPropArray[243] = new Int32JBoxProperty("/custom_properties/prop_array_244");
  fPropArray[244] = new Int32JBoxProperty("/custom_properties/prop_array_245");
  fPropArray[245] = new Int32JBoxProperty("/custom_properties/prop_array_246");
  fPropArray[246] = new Int32JBoxProperty("/custom_properties/prop_array_247");
  fPropArray[247] = new Int32JBoxProperty("/custom_properties/prop_array_248");
  fPropArray[248] = new Int32JBoxProperty("/custom_properties/prop_array_249");
  fPropArray[249] = new Int32JBoxProperty("/custom_properties/prop_array_250");
  fPropArray[250] = new Int32JBoxProperty("/custom_properties/prop_array_251");
  fPropArray[251] = new Int32JBoxProperty("/custom_properties/prop_array_252");
  fPropArray[252] = new Int32JBoxProperty("/custom_properties/prop_array_253");
  fPropArray[253] = new Int32JBoxProperty("/custom_properties/prop_array_254");
  fPropArray[254] = new Int32JBoxProperty("/custom_properties/prop_array_255");
  fPropArray[255] = new Int32JBoxProperty("/custom_properties/prop_array_256");
}
