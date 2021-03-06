// @copyright 2017-2018 zzu_softboy <zzu_softboy@163.com>
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Created by softboy on 2018/01/15.

// ****************************************************************************
// *                                                                          *
// *     (C) Copyright Paul Mensonides 2002-2011.                             *
// *     (C) Copyright Edward Diener 2011.                                    *
// *     Distributed under the Boost Software License, Version 1.0. (See      *
// *     accompanying file LICENSE_1_0.txt or copy at                         *
// *     http://www.boost.org/LICENSE_1_0.txt)                                *
// *                                                                          *
// ****************************************************************************
// 
// See http://www.boost.org for most recent version.

#ifndef PDK_STDEXT_PREPROCESSOR_INTERNAL_DMC_AUTO_REC_H
#define PDK_STDEXT_PREPROCESSOR_INTERNAL_DMC_AUTO_REC_H

#include "pdk/stdext/preprocessor/control/Iif.h"

// PDK_PP_AUTO_REC

# define PDK_PP_AUTO_REC(pred, n) PDK_PP_NODE_ENTRY_ ## n(pred)

# define PDK_PP_NODE_ENTRY_256(p) PDK_PP_NODE_128(p)(p)(p)(p)(p)(p)(p)(p)
# define PDK_PP_NODE_ENTRY_128(p) PDK_PP_NODE_64(p)(p)(p)(p)(p)(p)(p)
# define PDK_PP_NODE_ENTRY_64(p) PDK_PP_NODE_32(p)(p)(p)(p)(p)(p)
# define PDK_PP_NODE_ENTRY_32(p) PDK_PP_NODE_16(p)(p)(p)(p)(p)
# define PDK_PP_NODE_ENTRY_16(p) PDK_PP_NODE_8(p)(p)(p)(p)
# define PDK_PP_NODE_ENTRY_8(p) PDK_PP_NODE_4(p)(p)(p)
# define PDK_PP_NODE_ENTRY_4(p) PDK_PP_NODE_2(p)(p)
# define PDK_PP_NODE_ENTRY_2(p) PDK_PP_NODE_1(p)

# define PDK_PP_NODE_128(p) PDK_PP_IIF(p##(128), PDK_PP_NODE_64, PDK_PP_NODE_192)
#    define PDK_PP_NODE_64(p) PDK_PP_IIF(p##(64), PDK_PP_NODE_32, PDK_PP_NODE_96)
#        define PDK_PP_NODE_32(p) PDK_PP_IIF(p##(32), PDK_PP_NODE_16, PDK_PP_NODE_48)
#            define PDK_PP_NODE_16(p) PDK_PP_IIF(p##(16), PDK_PP_NODE_8, PDK_PP_NODE_24)
#                define PDK_PP_NODE_8(p) PDK_PP_IIF(p##(8), PDK_PP_NODE_4, PDK_PP_NODE_12)
#                    define PDK_PP_NODE_4(p) PDK_PP_IIF(p##(4), PDK_PP_NODE_2, PDK_PP_NODE_6)
#                        define PDK_PP_NODE_2(p) PDK_PP_IIF(p##(2), PDK_PP_NODE_1, PDK_PP_NODE_3)
#                            define PDK_PP_NODE_1(p) PDK_PP_IIF(p##(1), 1, 2)
#                            define PDK_PP_NODE_3(p) PDK_PP_IIF(p##(3), 3, 4)
#                        define PDK_PP_NODE_6(p) PDK_PP_IIF(p##(6), PDK_PP_NODE_5, PDK_PP_NODE_7)
#                            define PDK_PP_NODE_5(p) PDK_PP_IIF(p##(5), 5, 6)
#                            define PDK_PP_NODE_7(p) PDK_PP_IIF(p##(7), 7, 8)
#                    define PDK_PP_NODE_12(p) PDK_PP_IIF(p##(12), PDK_PP_NODE_10, PDK_PP_NODE_14)
#                        define PDK_PP_NODE_10(p) PDK_PP_IIF(p##(10), PDK_PP_NODE_9, PDK_PP_NODE_11)
#                            define PDK_PP_NODE_9(p) PDK_PP_IIF(p##(9), 9, 10)
#                            define PDK_PP_NODE_11(p) PDK_PP_IIF(p##(11), 11, 12)
#                        define PDK_PP_NODE_14(p) PDK_PP_IIF(p##(14), PDK_PP_NODE_13, PDK_PP_NODE_15)
#                            define PDK_PP_NODE_13(p) PDK_PP_IIF(p##(13), 13, 14)
#                            define PDK_PP_NODE_15(p) PDK_PP_IIF(p##(15), 15, 16)
#                define PDK_PP_NODE_24(p) PDK_PP_IIF(p##(24), PDK_PP_NODE_20, PDK_PP_NODE_28)
#                    define PDK_PP_NODE_20(p) PDK_PP_IIF(p##(20), PDK_PP_NODE_18, PDK_PP_NODE_22)
#                        define PDK_PP_NODE_18(p) PDK_PP_IIF(p##(18), PDK_PP_NODE_17, PDK_PP_NODE_19)
#                            define PDK_PP_NODE_17(p) PDK_PP_IIF(p##(17), 17, 18)
#                            define PDK_PP_NODE_19(p) PDK_PP_IIF(p##(19), 19, 20)
#                        define PDK_PP_NODE_22(p) PDK_PP_IIF(p##(22), PDK_PP_NODE_21, PDK_PP_NODE_23)
#                            define PDK_PP_NODE_21(p) PDK_PP_IIF(p##(21), 21, 22)
#                            define PDK_PP_NODE_23(p) PDK_PP_IIF(p##(23), 23, 24)
#                    define PDK_PP_NODE_28(p) PDK_PP_IIF(p##(28), PDK_PP_NODE_26, PDK_PP_NODE_30)
#                        define PDK_PP_NODE_26(p) PDK_PP_IIF(p##(26), PDK_PP_NODE_25, PDK_PP_NODE_27)
#                            define PDK_PP_NODE_25(p) PDK_PP_IIF(p##(25), 25, 26)
#                            define PDK_PP_NODE_27(p) PDK_PP_IIF(p##(27), 27, 28)
#                        define PDK_PP_NODE_30(p) PDK_PP_IIF(p##(30), PDK_PP_NODE_29, PDK_PP_NODE_31)
#                            define PDK_PP_NODE_29(p) PDK_PP_IIF(p##(29), 29, 30)
#                            define PDK_PP_NODE_31(p) PDK_PP_IIF(p##(31), 31, 32)
#            define PDK_PP_NODE_48(p) PDK_PP_IIF(p##(48), PDK_PP_NODE_40, PDK_PP_NODE_56)
#                define PDK_PP_NODE_40(p) PDK_PP_IIF(p##(40), PDK_PP_NODE_36, PDK_PP_NODE_44)
#                    define PDK_PP_NODE_36(p) PDK_PP_IIF(p##(36), PDK_PP_NODE_34, PDK_PP_NODE_38)
#                        define PDK_PP_NODE_34(p) PDK_PP_IIF(p##(34), PDK_PP_NODE_33, PDK_PP_NODE_35)
#                            define PDK_PP_NODE_33(p) PDK_PP_IIF(p##(33), 33, 34)
#                            define PDK_PP_NODE_35(p) PDK_PP_IIF(p##(35), 35, 36)
#                        define PDK_PP_NODE_38(p) PDK_PP_IIF(p##(38), PDK_PP_NODE_37, PDK_PP_NODE_39)
#                            define PDK_PP_NODE_37(p) PDK_PP_IIF(p##(37), 37, 38)
#                            define PDK_PP_NODE_39(p) PDK_PP_IIF(p##(39), 39, 40)
#                    define PDK_PP_NODE_44(p) PDK_PP_IIF(p##(44), PDK_PP_NODE_42, PDK_PP_NODE_46)
#                        define PDK_PP_NODE_42(p) PDK_PP_IIF(p##(42), PDK_PP_NODE_41, PDK_PP_NODE_43)
#                            define PDK_PP_NODE_41(p) PDK_PP_IIF(p##(41), 41, 42)
#                            define PDK_PP_NODE_43(p) PDK_PP_IIF(p##(43), 43, 44)
#                        define PDK_PP_NODE_46(p) PDK_PP_IIF(p##(46), PDK_PP_NODE_45, PDK_PP_NODE_47)
#                            define PDK_PP_NODE_45(p) PDK_PP_IIF(p##(45), 45, 46)
#                            define PDK_PP_NODE_47(p) PDK_PP_IIF(p##(47), 47, 48)
#                define PDK_PP_NODE_56(p) PDK_PP_IIF(p##(56), PDK_PP_NODE_52, PDK_PP_NODE_60)
#                    define PDK_PP_NODE_52(p) PDK_PP_IIF(p##(52), PDK_PP_NODE_50, PDK_PP_NODE_54)
#                        define PDK_PP_NODE_50(p) PDK_PP_IIF(p##(50), PDK_PP_NODE_49, PDK_PP_NODE_51)
#                            define PDK_PP_NODE_49(p) PDK_PP_IIF(p##(49), 49, 50)
#                            define PDK_PP_NODE_51(p) PDK_PP_IIF(p##(51), 51, 52)
#                        define PDK_PP_NODE_54(p) PDK_PP_IIF(p##(54), PDK_PP_NODE_53, PDK_PP_NODE_55)
#                            define PDK_PP_NODE_53(p) PDK_PP_IIF(p##(53), 53, 54)
#                            define PDK_PP_NODE_55(p) PDK_PP_IIF(p##(55), 55, 56)
#                    define PDK_PP_NODE_60(p) PDK_PP_IIF(p##(60), PDK_PP_NODE_58, PDK_PP_NODE_62)
#                        define PDK_PP_NODE_58(p) PDK_PP_IIF(p##(58), PDK_PP_NODE_57, PDK_PP_NODE_59)
#                            define PDK_PP_NODE_57(p) PDK_PP_IIF(p##(57), 57, 58)
#                            define PDK_PP_NODE_59(p) PDK_PP_IIF(p##(59), 59, 60)
#                        define PDK_PP_NODE_62(p) PDK_PP_IIF(p##(62), PDK_PP_NODE_61, PDK_PP_NODE_63)
#                            define PDK_PP_NODE_61(p) PDK_PP_IIF(p##(61), 61, 62)
#                            define PDK_PP_NODE_63(p) PDK_PP_IIF(p##(63), 63, 64)
#        define PDK_PP_NODE_96(p) PDK_PP_IIF(p##(96), PDK_PP_NODE_80, PDK_PP_NODE_112)
#            define PDK_PP_NODE_80(p) PDK_PP_IIF(p##(80), PDK_PP_NODE_72, PDK_PP_NODE_88)
#                define PDK_PP_NODE_72(p) PDK_PP_IIF(p##(72), PDK_PP_NODE_68, PDK_PP_NODE_76)
#                    define PDK_PP_NODE_68(p) PDK_PP_IIF(p##(68), PDK_PP_NODE_66, PDK_PP_NODE_70)
#                        define PDK_PP_NODE_66(p) PDK_PP_IIF(p##(66), PDK_PP_NODE_65, PDK_PP_NODE_67)
#                            define PDK_PP_NODE_65(p) PDK_PP_IIF(p##(65), 65, 66)
#                            define PDK_PP_NODE_67(p) PDK_PP_IIF(p##(67), 67, 68)
#                        define PDK_PP_NODE_70(p) PDK_PP_IIF(p##(70), PDK_PP_NODE_69, PDK_PP_NODE_71)
#                            define PDK_PP_NODE_69(p) PDK_PP_IIF(p##(69), 69, 70)
#                            define PDK_PP_NODE_71(p) PDK_PP_IIF(p##(71), 71, 72)
#                    define PDK_PP_NODE_76(p) PDK_PP_IIF(p##(76), PDK_PP_NODE_74, PDK_PP_NODE_78)
#                        define PDK_PP_NODE_74(p) PDK_PP_IIF(p##(74), PDK_PP_NODE_73, PDK_PP_NODE_75)
#                            define PDK_PP_NODE_73(p) PDK_PP_IIF(p##(73), 73, 74)
#                            define PDK_PP_NODE_75(p) PDK_PP_IIF(p##(75), 75, 76)
#                        define PDK_PP_NODE_78(p) PDK_PP_IIF(p##(78), PDK_PP_NODE_77, PDK_PP_NODE_79)
#                            define PDK_PP_NODE_77(p) PDK_PP_IIF(p##(77), 77, 78)
#                            define PDK_PP_NODE_79(p) PDK_PP_IIF(p##(79), 79, 80)
#                define PDK_PP_NODE_88(p) PDK_PP_IIF(p##(88), PDK_PP_NODE_84, PDK_PP_NODE_92)
#                    define PDK_PP_NODE_84(p) PDK_PP_IIF(p##(84), PDK_PP_NODE_82, PDK_PP_NODE_86)
#                        define PDK_PP_NODE_82(p) PDK_PP_IIF(p##(82), PDK_PP_NODE_81, PDK_PP_NODE_83)
#                            define PDK_PP_NODE_81(p) PDK_PP_IIF(p##(81), 81, 82)
#                            define PDK_PP_NODE_83(p) PDK_PP_IIF(p##(83), 83, 84)
#                        define PDK_PP_NODE_86(p) PDK_PP_IIF(p##(86), PDK_PP_NODE_85, PDK_PP_NODE_87)
#                            define PDK_PP_NODE_85(p) PDK_PP_IIF(p##(85), 85, 86)
#                            define PDK_PP_NODE_87(p) PDK_PP_IIF(p##(87), 87, 88)
#                    define PDK_PP_NODE_92(p) PDK_PP_IIF(p##(92), PDK_PP_NODE_90, PDK_PP_NODE_94)
#                        define PDK_PP_NODE_90(p) PDK_PP_IIF(p##(90), PDK_PP_NODE_89, PDK_PP_NODE_91)
#                            define PDK_PP_NODE_89(p) PDK_PP_IIF(p##(89), 89, 90)
#                            define PDK_PP_NODE_91(p) PDK_PP_IIF(p##(91), 91, 92)
#                        define PDK_PP_NODE_94(p) PDK_PP_IIF(p##(94), PDK_PP_NODE_93, PDK_PP_NODE_95)
#                            define PDK_PP_NODE_93(p) PDK_PP_IIF(p##(93), 93, 94)
#                            define PDK_PP_NODE_95(p) PDK_PP_IIF(p##(95), 95, 96)
#            define PDK_PP_NODE_112(p) PDK_PP_IIF(p##(112), PDK_PP_NODE_104, PDK_PP_NODE_120)
#                define PDK_PP_NODE_104(p) PDK_PP_IIF(p##(104), PDK_PP_NODE_100, PDK_PP_NODE_108)
#                    define PDK_PP_NODE_100(p) PDK_PP_IIF(p##(100), PDK_PP_NODE_98, PDK_PP_NODE_102)
#                        define PDK_PP_NODE_98(p) PDK_PP_IIF(p##(98), PDK_PP_NODE_97, PDK_PP_NODE_99)
#                            define PDK_PP_NODE_97(p) PDK_PP_IIF(p##(97), 97, 98)
#                            define PDK_PP_NODE_99(p) PDK_PP_IIF(p##(99), 99, 100)
#                        define PDK_PP_NODE_102(p) PDK_PP_IIF(p##(102), PDK_PP_NODE_101, PDK_PP_NODE_103)
#                            define PDK_PP_NODE_101(p) PDK_PP_IIF(p##(101), 101, 102)
#                            define PDK_PP_NODE_103(p) PDK_PP_IIF(p##(103), 103, 104)
#                    define PDK_PP_NODE_108(p) PDK_PP_IIF(p##(108), PDK_PP_NODE_106, PDK_PP_NODE_110)
#                        define PDK_PP_NODE_106(p) PDK_PP_IIF(p##(106), PDK_PP_NODE_105, PDK_PP_NODE_107)
#                            define PDK_PP_NODE_105(p) PDK_PP_IIF(p##(105), 105, 106)
#                            define PDK_PP_NODE_107(p) PDK_PP_IIF(p##(107), 107, 108)
#                        define PDK_PP_NODE_110(p) PDK_PP_IIF(p##(110), PDK_PP_NODE_109, PDK_PP_NODE_111)
#                            define PDK_PP_NODE_109(p) PDK_PP_IIF(p##(109), 109, 110)
#                            define PDK_PP_NODE_111(p) PDK_PP_IIF(p##(111), 111, 112)
#                define PDK_PP_NODE_120(p) PDK_PP_IIF(p##(120), PDK_PP_NODE_116, PDK_PP_NODE_124)
#                    define PDK_PP_NODE_116(p) PDK_PP_IIF(p##(116), PDK_PP_NODE_114, PDK_PP_NODE_118)
#                        define PDK_PP_NODE_114(p) PDK_PP_IIF(p##(114), PDK_PP_NODE_113, PDK_PP_NODE_115)
#                            define PDK_PP_NODE_113(p) PDK_PP_IIF(p##(113), 113, 114)
#                            define PDK_PP_NODE_115(p) PDK_PP_IIF(p##(115), 115, 116)
#                        define PDK_PP_NODE_118(p) PDK_PP_IIF(p##(118), PDK_PP_NODE_117, PDK_PP_NODE_119)
#                            define PDK_PP_NODE_117(p) PDK_PP_IIF(p##(117), 117, 118)
#                            define PDK_PP_NODE_119(p) PDK_PP_IIF(p##(119), 119, 120)
#                    define PDK_PP_NODE_124(p) PDK_PP_IIF(p##(124), PDK_PP_NODE_122, PDK_PP_NODE_126)
#                        define PDK_PP_NODE_122(p) PDK_PP_IIF(p##(122), PDK_PP_NODE_121, PDK_PP_NODE_123)
#                            define PDK_PP_NODE_121(p) PDK_PP_IIF(p##(121), 121, 122)
#                            define PDK_PP_NODE_123(p) PDK_PP_IIF(p##(123), 123, 124)
#                        define PDK_PP_NODE_126(p) PDK_PP_IIF(p##(126), PDK_PP_NODE_125, PDK_PP_NODE_127)
#                            define PDK_PP_NODE_125(p) PDK_PP_IIF(p##(125), 125, 126)
#                            define PDK_PP_NODE_127(p) PDK_PP_IIF(p##(127), 127, 128)
#    define PDK_PP_NODE_192(p) PDK_PP_IIF(p##(192), PDK_PP_NODE_160, PDK_PP_NODE_224)
#        define PDK_PP_NODE_160(p) PDK_PP_IIF(p##(160), PDK_PP_NODE_144, PDK_PP_NODE_176)
#            define PDK_PP_NODE_144(p) PDK_PP_IIF(p##(144), PDK_PP_NODE_136, PDK_PP_NODE_152)
#                define PDK_PP_NODE_136(p) PDK_PP_IIF(p##(136), PDK_PP_NODE_132, PDK_PP_NODE_140)
#                    define PDK_PP_NODE_132(p) PDK_PP_IIF(p##(132), PDK_PP_NODE_130, PDK_PP_NODE_134)
#                        define PDK_PP_NODE_130(p) PDK_PP_IIF(p##(130), PDK_PP_NODE_129, PDK_PP_NODE_131)
#                            define PDK_PP_NODE_129(p) PDK_PP_IIF(p##(129), 129, 130)
#                            define PDK_PP_NODE_131(p) PDK_PP_IIF(p##(131), 131, 132)
#                        define PDK_PP_NODE_134(p) PDK_PP_IIF(p##(134), PDK_PP_NODE_133, PDK_PP_NODE_135)
#                            define PDK_PP_NODE_133(p) PDK_PP_IIF(p##(133), 133, 134)
#                            define PDK_PP_NODE_135(p) PDK_PP_IIF(p##(135), 135, 136)
#                    define PDK_PP_NODE_140(p) PDK_PP_IIF(p##(140), PDK_PP_NODE_138, PDK_PP_NODE_142)
#                        define PDK_PP_NODE_138(p) PDK_PP_IIF(p##(138), PDK_PP_NODE_137, PDK_PP_NODE_139)
#                            define PDK_PP_NODE_137(p) PDK_PP_IIF(p##(137), 137, 138)
#                            define PDK_PP_NODE_139(p) PDK_PP_IIF(p##(139), 139, 140)
#                        define PDK_PP_NODE_142(p) PDK_PP_IIF(p##(142), PDK_PP_NODE_141, PDK_PP_NODE_143)
#                            define PDK_PP_NODE_141(p) PDK_PP_IIF(p##(141), 141, 142)
#                            define PDK_PP_NODE_143(p) PDK_PP_IIF(p##(143), 143, 144)
#                define PDK_PP_NODE_152(p) PDK_PP_IIF(p##(152), PDK_PP_NODE_148, PDK_PP_NODE_156)
#                    define PDK_PP_NODE_148(p) PDK_PP_IIF(p##(148), PDK_PP_NODE_146, PDK_PP_NODE_150)
#                        define PDK_PP_NODE_146(p) PDK_PP_IIF(p##(146), PDK_PP_NODE_145, PDK_PP_NODE_147)
#                            define PDK_PP_NODE_145(p) PDK_PP_IIF(p##(145), 145, 146)
#                            define PDK_PP_NODE_147(p) PDK_PP_IIF(p##(147), 147, 148)
#                        define PDK_PP_NODE_150(p) PDK_PP_IIF(p##(150), PDK_PP_NODE_149, PDK_PP_NODE_151)
#                            define PDK_PP_NODE_149(p) PDK_PP_IIF(p##(149), 149, 150)
#                            define PDK_PP_NODE_151(p) PDK_PP_IIF(p##(151), 151, 152)
#                    define PDK_PP_NODE_156(p) PDK_PP_IIF(p##(156), PDK_PP_NODE_154, PDK_PP_NODE_158)
#                        define PDK_PP_NODE_154(p) PDK_PP_IIF(p##(154), PDK_PP_NODE_153, PDK_PP_NODE_155)
#                            define PDK_PP_NODE_153(p) PDK_PP_IIF(p##(153), 153, 154)
#                            define PDK_PP_NODE_155(p) PDK_PP_IIF(p##(155), 155, 156)
#                        define PDK_PP_NODE_158(p) PDK_PP_IIF(p##(158), PDK_PP_NODE_157, PDK_PP_NODE_159)
#                            define PDK_PP_NODE_157(p) PDK_PP_IIF(p##(157), 157, 158)
#                            define PDK_PP_NODE_159(p) PDK_PP_IIF(p##(159), 159, 160)
#            define PDK_PP_NODE_176(p) PDK_PP_IIF(p##(176), PDK_PP_NODE_168, PDK_PP_NODE_184)
#                define PDK_PP_NODE_168(p) PDK_PP_IIF(p##(168), PDK_PP_NODE_164, PDK_PP_NODE_172)
#                    define PDK_PP_NODE_164(p) PDK_PP_IIF(p##(164), PDK_PP_NODE_162, PDK_PP_NODE_166)
#                        define PDK_PP_NODE_162(p) PDK_PP_IIF(p##(162), PDK_PP_NODE_161, PDK_PP_NODE_163)
#                            define PDK_PP_NODE_161(p) PDK_PP_IIF(p##(161), 161, 162)
#                            define PDK_PP_NODE_163(p) PDK_PP_IIF(p##(163), 163, 164)
#                        define PDK_PP_NODE_166(p) PDK_PP_IIF(p##(166), PDK_PP_NODE_165, PDK_PP_NODE_167)
#                            define PDK_PP_NODE_165(p) PDK_PP_IIF(p##(165), 165, 166)
#                            define PDK_PP_NODE_167(p) PDK_PP_IIF(p##(167), 167, 168)
#                    define PDK_PP_NODE_172(p) PDK_PP_IIF(p##(172), PDK_PP_NODE_170, PDK_PP_NODE_174)
#                        define PDK_PP_NODE_170(p) PDK_PP_IIF(p##(170), PDK_PP_NODE_169, PDK_PP_NODE_171)
#                            define PDK_PP_NODE_169(p) PDK_PP_IIF(p##(169), 169, 170)
#                            define PDK_PP_NODE_171(p) PDK_PP_IIF(p##(171), 171, 172)
#                        define PDK_PP_NODE_174(p) PDK_PP_IIF(p##(174), PDK_PP_NODE_173, PDK_PP_NODE_175)
#                            define PDK_PP_NODE_173(p) PDK_PP_IIF(p##(173), 173, 174)
#                            define PDK_PP_NODE_175(p) PDK_PP_IIF(p##(175), 175, 176)
#                define PDK_PP_NODE_184(p) PDK_PP_IIF(p##(184), PDK_PP_NODE_180, PDK_PP_NODE_188)
#                    define PDK_PP_NODE_180(p) PDK_PP_IIF(p##(180), PDK_PP_NODE_178, PDK_PP_NODE_182)
#                        define PDK_PP_NODE_178(p) PDK_PP_IIF(p##(178), PDK_PP_NODE_177, PDK_PP_NODE_179)
#                            define PDK_PP_NODE_177(p) PDK_PP_IIF(p##(177), 177, 178)
#                            define PDK_PP_NODE_179(p) PDK_PP_IIF(p##(179), 179, 180)
#                        define PDK_PP_NODE_182(p) PDK_PP_IIF(p##(182), PDK_PP_NODE_181, PDK_PP_NODE_183)
#                            define PDK_PP_NODE_181(p) PDK_PP_IIF(p##(181), 181, 182)
#                            define PDK_PP_NODE_183(p) PDK_PP_IIF(p##(183), 183, 184)
#                    define PDK_PP_NODE_188(p) PDK_PP_IIF(p##(188), PDK_PP_NODE_186, PDK_PP_NODE_190)
#                        define PDK_PP_NODE_186(p) PDK_PP_IIF(p##(186), PDK_PP_NODE_185, PDK_PP_NODE_187)
#                            define PDK_PP_NODE_185(p) PDK_PP_IIF(p##(185), 185, 186)
#                            define PDK_PP_NODE_187(p) PDK_PP_IIF(p##(187), 187, 188)
#                        define PDK_PP_NODE_190(p) PDK_PP_IIF(p##(190), PDK_PP_NODE_189, PDK_PP_NODE_191)
#                            define PDK_PP_NODE_189(p) PDK_PP_IIF(p##(189), 189, 190)
#                            define PDK_PP_NODE_191(p) PDK_PP_IIF(p##(191), 191, 192)
#        define PDK_PP_NODE_224(p) PDK_PP_IIF(p##(224), PDK_PP_NODE_208, PDK_PP_NODE_240)
#            define PDK_PP_NODE_208(p) PDK_PP_IIF(p##(208), PDK_PP_NODE_200, PDK_PP_NODE_216)
#                define PDK_PP_NODE_200(p) PDK_PP_IIF(p##(200), PDK_PP_NODE_196, PDK_PP_NODE_204)
#                    define PDK_PP_NODE_196(p) PDK_PP_IIF(p##(196), PDK_PP_NODE_194, PDK_PP_NODE_198)
#                        define PDK_PP_NODE_194(p) PDK_PP_IIF(p##(194), PDK_PP_NODE_193, PDK_PP_NODE_195)
#                            define PDK_PP_NODE_193(p) PDK_PP_IIF(p##(193), 193, 194)
#                            define PDK_PP_NODE_195(p) PDK_PP_IIF(p##(195), 195, 196)
#                        define PDK_PP_NODE_198(p) PDK_PP_IIF(p##(198), PDK_PP_NODE_197, PDK_PP_NODE_199)
#                            define PDK_PP_NODE_197(p) PDK_PP_IIF(p##(197), 197, 198)
#                            define PDK_PP_NODE_199(p) PDK_PP_IIF(p##(199), 199, 200)
#                    define PDK_PP_NODE_204(p) PDK_PP_IIF(p##(204), PDK_PP_NODE_202, PDK_PP_NODE_206)
#                        define PDK_PP_NODE_202(p) PDK_PP_IIF(p##(202), PDK_PP_NODE_201, PDK_PP_NODE_203)
#                            define PDK_PP_NODE_201(p) PDK_PP_IIF(p##(201), 201, 202)
#                            define PDK_PP_NODE_203(p) PDK_PP_IIF(p##(203), 203, 204)
#                        define PDK_PP_NODE_206(p) PDK_PP_IIF(p##(206), PDK_PP_NODE_205, PDK_PP_NODE_207)
#                            define PDK_PP_NODE_205(p) PDK_PP_IIF(p##(205), 205, 206)
#                            define PDK_PP_NODE_207(p) PDK_PP_IIF(p##(207), 207, 208)
#                define PDK_PP_NODE_216(p) PDK_PP_IIF(p##(216), PDK_PP_NODE_212, PDK_PP_NODE_220)
#                    define PDK_PP_NODE_212(p) PDK_PP_IIF(p##(212), PDK_PP_NODE_210, PDK_PP_NODE_214)
#                        define PDK_PP_NODE_210(p) PDK_PP_IIF(p##(210), PDK_PP_NODE_209, PDK_PP_NODE_211)
#                            define PDK_PP_NODE_209(p) PDK_PP_IIF(p##(209), 209, 210)
#                            define PDK_PP_NODE_211(p) PDK_PP_IIF(p##(211), 211, 212)
#                        define PDK_PP_NODE_214(p) PDK_PP_IIF(p##(214), PDK_PP_NODE_213, PDK_PP_NODE_215)
#                            define PDK_PP_NODE_213(p) PDK_PP_IIF(p##(213), 213, 214)
#                            define PDK_PP_NODE_215(p) PDK_PP_IIF(p##(215), 215, 216)
#                    define PDK_PP_NODE_220(p) PDK_PP_IIF(p##(220), PDK_PP_NODE_218, PDK_PP_NODE_222)
#                        define PDK_PP_NODE_218(p) PDK_PP_IIF(p##(218), PDK_PP_NODE_217, PDK_PP_NODE_219)
#                            define PDK_PP_NODE_217(p) PDK_PP_IIF(p##(217), 217, 218)
#                            define PDK_PP_NODE_219(p) PDK_PP_IIF(p##(219), 219, 220)
#                        define PDK_PP_NODE_222(p) PDK_PP_IIF(p##(222), PDK_PP_NODE_221, PDK_PP_NODE_223)
#                            define PDK_PP_NODE_221(p) PDK_PP_IIF(p##(221), 221, 222)
#                            define PDK_PP_NODE_223(p) PDK_PP_IIF(p##(223), 223, 224)
#            define PDK_PP_NODE_240(p) PDK_PP_IIF(p##(240), PDK_PP_NODE_232, PDK_PP_NODE_248)
#                define PDK_PP_NODE_232(p) PDK_PP_IIF(p##(232), PDK_PP_NODE_228, PDK_PP_NODE_236)
#                    define PDK_PP_NODE_228(p) PDK_PP_IIF(p##(228), PDK_PP_NODE_226, PDK_PP_NODE_230)
#                        define PDK_PP_NODE_226(p) PDK_PP_IIF(p##(226), PDK_PP_NODE_225, PDK_PP_NODE_227)
#                            define PDK_PP_NODE_225(p) PDK_PP_IIF(p##(225), 225, 226)
#                            define PDK_PP_NODE_227(p) PDK_PP_IIF(p##(227), 227, 228)
#                        define PDK_PP_NODE_230(p) PDK_PP_IIF(p##(230), PDK_PP_NODE_229, PDK_PP_NODE_231)
#                            define PDK_PP_NODE_229(p) PDK_PP_IIF(p##(229), 229, 230)
#                            define PDK_PP_NODE_231(p) PDK_PP_IIF(p##(231), 231, 232)
#                    define PDK_PP_NODE_236(p) PDK_PP_IIF(p##(236), PDK_PP_NODE_234, PDK_PP_NODE_238)
#                        define PDK_PP_NODE_234(p) PDK_PP_IIF(p##(234), PDK_PP_NODE_233, PDK_PP_NODE_235)
#                            define PDK_PP_NODE_233(p) PDK_PP_IIF(p##(233), 233, 234)
#                            define PDK_PP_NODE_235(p) PDK_PP_IIF(p##(235), 235, 236)
#                        define PDK_PP_NODE_238(p) PDK_PP_IIF(p##(238), PDK_PP_NODE_237, PDK_PP_NODE_239)
#                            define PDK_PP_NODE_237(p) PDK_PP_IIF(p##(237), 237, 238)
#                            define PDK_PP_NODE_239(p) PDK_PP_IIF(p##(239), 239, 240)
#                define PDK_PP_NODE_248(p) PDK_PP_IIF(p##(248), PDK_PP_NODE_244, PDK_PP_NODE_252)
#                    define PDK_PP_NODE_244(p) PDK_PP_IIF(p##(244), PDK_PP_NODE_242, PDK_PP_NODE_246)
#                        define PDK_PP_NODE_242(p) PDK_PP_IIF(p##(242), PDK_PP_NODE_241, PDK_PP_NODE_243)
#                            define PDK_PP_NODE_241(p) PDK_PP_IIF(p##(241), 241, 242)
#                            define PDK_PP_NODE_243(p) PDK_PP_IIF(p##(243), 243, 244)
#                        define PDK_PP_NODE_246(p) PDK_PP_IIF(p##(246), PDK_PP_NODE_245, PDK_PP_NODE_247)
#                            define PDK_PP_NODE_245(p) PDK_PP_IIF(p##(245), 245, 246)
#                            define PDK_PP_NODE_247(p) PDK_PP_IIF(p##(247), 247, 248)
#                    define PDK_PP_NODE_252(p) PDK_PP_IIF(p##(252), PDK_PP_NODE_250, PDK_PP_NODE_254)
#                        define PDK_PP_NODE_250(p) PDK_PP_IIF(p##(250), PDK_PP_NODE_249, PDK_PP_NODE_251)
#                            define PDK_PP_NODE_249(p) PDK_PP_IIF(p##(249), 249, 250)
#                            define PDK_PP_NODE_251(p) PDK_PP_IIF(p##(251), 251, 252)
#                        define PDK_PP_NODE_254(p) PDK_PP_IIF(p##(254), PDK_PP_NODE_253, PDK_PP_NODE_255)
#                            define PDK_PP_NODE_253(p) PDK_PP_IIF(p##(253), 253, 254)
#                            define PDK_PP_NODE_255(p) PDK_PP_IIF(p##(255), 255, 256)
#
# endif

#endif // PDK_STDEXT_PREPROCESSOR_INTERNAL_DMC_AUTO_REC_H
