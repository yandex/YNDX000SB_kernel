#ifndef FS16XX_REGS_H
#define FS16XX_REGS_H

#define FS16XX_OTP_ACC_KEY   0xCA00
#define FS16XX_OTP_ACC_KEY2  0xCA91
#define FS11601_OTP_BST_CFG  0xC5FC

#define FS16XX_STATUS_OK_MASK   ( FS16XX_STATUS_REG_CLKS| FS16XX_STATUS_REG_UVDS | FS16XX_STATUS_REG_OVDS \
                                | FS16XX_STATUS_REG_OTDS | FS16XX_STATUS_REG_PLLS | FS16XX_STATUS_REG_VDDS)

#define FS1601S_DSP_DCCOEF_DEFAULT 0x02
#define FS16XX_DSP_ENABLE_ALL_MASK  (FS16XX_DSPCTRL_REG_DSPEN | FS16XX_DSPCTRL_REG_EQEN \
                                | FS16XX_DSPCTRL_REG_EQBEN_MSK | FS16XX_DSPCTRL_REG_ACSEQEN \
                                | FS16XX_DSPCTRL_REG_NOFILTEN | FS1601S_DSP_DCCOEF_DEFAULT)

#define FS16XX_DSP_BYPASS_ALL_MASK  0x04

#define FS16XX_ADCCTRL_REG_DEFAULT  0x1300
#define FS16XX_ADCCTRL_REG_DISABLE  0x0300

#define FS16XX_PLLCTRL_EN_ALL (FS16XX_PLLCTRL4_REG_PLLEN | FS16XX_PLLCTRL4_REG_OSCEN \
                                | FS16XX_PLLCTRL4_REG_ZMEN | FS16XX_PLLCTRL4_REG_VBGEN)

/*************** STATUS (0x00) ***************/
#define FS16XX_STATUS_REG                0x00

#define FS16XX_STATUS_REG_VDDS               (0x01<<0)
#define FS16XX_STATUS_REG_VDDS_POS           0
#define FS16XX_STATUS_REG_VDDS_LEN           1
#define FS16XX_STATUS_REG_VDDS_MSK           0x1
#define FS16XX_STATUS_REG_PLLS               (0x01<<1)
#define FS16XX_STATUS_REG_PLLS_POS           1
#define FS16XX_STATUS_REG_PLLS_LEN           1
#define FS16XX_STATUS_REG_PLLS_MSK           0x2
#define FS16XX_STATUS_REG_OTDS               (0x01<<2)
#define FS16XX_STATUS_REG_OTDS_POS           2
#define FS16XX_STATUS_REG_OTDS_LEN           1
#define FS16XX_STATUS_REG_OTDS_MSK           0x4
#define FS16XX_STATUS_REG_OVDS               (0x01<<3)
#define FS16XX_STATUS_REG_OVDS_POS           3
#define FS16XX_STATUS_REG_OVDS_LEN           1
#define FS16XX_STATUS_REG_OVDS_MSK           0x8
#define FS16XX_STATUS_REG_UVDS               (0x01<<4)
#define FS16XX_STATUS_REG_UVDS_POS           4
#define FS16XX_STATUS_REG_UVDS_LEN           1
#define FS16XX_STATUS_REG_UVDS_MSK           0x10
#define FS16XX_STATUS_REG_OCDS               (0x01<<5)
#define FS16XX_STATUS_REG_OCDS_POS           5
#define FS16XX_STATUS_REG_OCDS_LEN           1
#define FS16XX_STATUS_REG_OCDS_MSK           0x20
#define FS16XX_STATUS_REG_CLKS               (0x01<<6)
#define FS16XX_STATUS_REG_CLKS_POS           6
#define FS16XX_STATUS_REG_CLKS_LEN           1
#define FS16XX_STATUS_REG_CLKS_MSK           0x40
#define FS16XX_STATUS_REG_SPKS               (0x01<<10)
#define FS16XX_STATUS_REG_SPKS_POS           10
#define FS16XX_STATUS_REG_SPKS_LEN           1
#define FS16XX_STATUS_REG_SPKS_MSK           0x400

/*************** BATS (0x01) ***************/
#define FS16XX_BATS_REG                0x01

#define FS16XX_BATS_REG_BATV                 (0x01<<0)
#define FS16XX_BATS_REG_BATV_POS             0
#define FS16XX_BATS_REG_BATV_LEN             10
#define FS16XX_BATS_REG_BATV_MSK             0x3ff

/*************** TEMPS (0x02) ***************/
#define FS16XX_TEMPS_REG                0x02

#define FS16XX_TEMPS_REG_TEMPV               (0x01<<0)
#define FS16XX_TEMPS_REG_TEMPV_POS           0
#define FS16XX_TEMPS_REG_TEMPV_LEN           9
#define FS16XX_TEMPS_REG_TEMPV_MSK           0x1ff

/*************** ID (0x03) ***************/
#define FS16XX_ID_REG                0x03

#define FS16XX_ID_REG_REV                    (0x01<<0)
#define FS16XX_ID_REG_REV_POS                0
#define FS16XX_ID_REG_REV_LEN                8
#define FS16XX_ID_REG_REV_MSK                0xff
#define FS16XX_ID_REG_DEVICE_ID              (0x01<<8)
#define FS16XX_ID_REG_DEVICE_ID_POS          8
#define FS16XX_ID_REG_DEVICE_ID_LEN          8
#define FS16XX_ID_REG_DEVICE_ID_MSK          0xff00

/*************** I2SCTRL (0x04) ***************/
#define FS16XX_I2SCTRL_REG                0x04

#define FS16XX_I2SCTRL_REG_I2SF              (0x01<<0)
#define FS16XX_I2SCTRL_REG_I2SF_POS          0
#define FS16XX_I2SCTRL_REG_I2SF_LEN          3
#define FS16XX_I2SCTRL_REG_I2SF_MSK          0x7
#define FS16XX_I2SCTRL_REG_CHS12             (0x01<<3)
#define FS16XX_I2SCTRL_REG_CHS12_POS         3
#define FS16XX_I2SCTRL_REG_CHS12_POS1 		 4
#define FS16XX_I2SCTRL_REG_CHS12_LEN         2
#define FS16XX_I2SCTRL_REG_CHS12_MSK         0x18
#define FS16XX_I2SCTRL_REG_DISP              (0x01<<10)
#define FS16XX_I2SCTRL_REG_DISP_POS          10
#define FS16XX_I2SCTRL_REG_DISP_LEN          1
#define FS16XX_I2SCTRL_REG_DISP_MSK          0x400
#define FS16XX_I2SCTRL_REG_I2SDOE            (0x01<<11)
#define FS16XX_I2SCTRL_REG_I2SDOE_POS        11
#define FS16XX_I2SCTRL_REG_I2SDOE_LEN        1
#define FS16XX_I2SCTRL_REG_I2SDOE_MSK        0x800
#define FS16XX_I2SCTRL_REG_I2SSR             (0x01<<12)
#define FS16XX_I2SCTRL_REG_I2SSR_POS         12
#define FS16XX_I2SCTRL_REG_I2SSR_LEN         4
#define FS16XX_I2SCTRL_REG_I2SSR_MSK         0xf000

/*************** AUDIOCTRL (0x06) ***************/
#define FS16XX_AUDIOCTRL_REG                0x06

#define FS16XX_AUDIOCTRL_REG_VOL             (0x01<<8)
#define FS16XX_AUDIOCTRL_REG_VOL_POS         8
#define FS16XX_AUDIOCTRL_REG_VOL_LEN         8
#define FS16XX_AUDIOCTRL_REG_VOL_MSK         0xff00

/*************** TEMPSEL (0x08) ***************/
#define FS16XX_TEMPSEL_REG                0x08

#define FS16XX_TEMPSEL_REG_EXTTS             (0x01<<1)
#define FS16XX_TEMPSEL_REG_EXTTS_POS         1
#define FS16XX_TEMPSEL_REG_EXTTS_LEN         9
#define FS16XX_TEMPSEL_REG_EXTTS_MSK         0x3fe

/*************** SYSCTRL (0x09) ***************/
#define FS16XX_SYSCTRL_REG                0x09

#define FS16XX_SYSCTRL_REG_PWDN              (0x01<<0)
#define FS16XX_SYSCTRL_REG_PWDN_POS          0
#define FS16XX_SYSCTRL_REG_PWDN_LEN          1
#define FS16XX_SYSCTRL_REG_PWDN_MSK          0x1
#define FS16XX_SYSCTRL_REG_I2CR              (0x01<<1)
#define FS16XX_SYSCTRL_REG_I2CR_POS          1
#define FS16XX_SYSCTRL_REG_I2CR_LEN          1
#define FS16XX_SYSCTRL_REG_I2CR_MSK          0x2
#define FS16XX_SYSCTRL_REG_AMPE              (0x01<<3)
#define FS16XX_SYSCTRL_REG_AMPE_POS          3
#define FS16XX_SYSCTRL_REG_AMPE_LEN          1
#define FS16XX_SYSCTRL_REG_AMPE_MSK          0x8

/*************** SPKSET (0x0A) ***************/
#define FS16XX_SPKSET_REG                0x0A

#define FS16XX_SPKSET_REG_SPKR               (0x01<<9)
#define FS16XX_SPKSET_REG_SPKR_POS           9
#define FS16XX_SPKSET_REG_SPKR_LEN           2
#define FS16XX_SPKSET_REG_SPKR_MSK           0x600

/*************** OTPACC (0x0B) ***************/
#define FS16XX_OTPACC_REG                0x0B

#define FS16XX_OTPACC_REG_TRIMKEY            (0x01<<0)
#define FS16XX_OTPACC_REG_TRIMKEY_POS        0
#define FS16XX_OTPACC_REG_TRIMKEY_LEN        8
#define FS16XX_OTPACC_REG_TRIMKEY_MSK        0xff
#define FS16XX_OTPACC_REG_REKEY              (0x01<<8)
#define FS16XX_OTPACC_REG_REKEY_POS          8
#define FS16XX_OTPACC_REG_REKEY_LEN          8
#define FS16XX_OTPACC_REG_REKEY_MSK          0xff00

/*************** SPKSTAUTS (0x80) ***************/
#define FS16XX_SPKSTAUTS_REG                0x80

#define FS16XX_SPKSTAUTS_REG_MTPEX           (0x01<<1)
#define FS16XX_SPKSTAUTS_REG_MTPEX_POS       1
#define FS16XX_SPKSTAUTS_REG_MTPEX_LEN       1
#define FS16XX_SPKSTAUTS_REG_MTPEX_MSK       0x2

/*************** CHIPINI (0x90) ***************/
#define FS16XX_CHIPINI_REG                0x90

#define FS16XX_CHIPINI_REG_INIFINISH         (0x01<<0)
#define FS16XX_CHIPINI_REG_INIFINISH_POS     0
#define FS16XX_CHIPINI_REG_INIFINISH_LEN     1
#define FS16XX_CHIPINI_REG_INIFINISH_MSK     0x1
#define FS16XX_CHIPINI_REG_INIOK             (0x01<<1)
#define FS16XX_CHIPINI_REG_INIOK_POS         1
#define FS16XX_CHIPINI_REG_INIOK_LEN         1
#define FS16XX_CHIPINI_REG_INIOK_MSK         0x2

/*************** I2CADD (0x91) ***************/
#define FS16XX_I2CADD_REG                0x91

#define FS16XX_I2CADD_REG_ADR                (0x01<<0)
#define FS16XX_I2CADD_REG_ADR_POS            0
#define FS16XX_I2CADD_REG_ADR_LEN            7
#define FS16XX_I2CADD_REG_ADR_MSK            0x7f

/*************** AUTOCTRL (0x97) ***************/
#define FS16XX_AUTOCTRL_REG                0x97

#define FS16XX_AUTOCTRL_REG_SILETHRD         (0x01<<0)
#define FS16XX_AUTOCTRL_REG_SILETHRD_POS     0
#define FS16XX_AUTOCTRL_REG_SILETHRD_LEN     8
#define FS16XX_AUTOCTRL_REG_SILETHRD_MSK     0xff
#define FS16XX_AUTOCTRL_REG_AUTOPD           (0x01<<8)
#define FS16XX_AUTOCTRL_REG_AUTOPD_POS       8
#define FS16XX_AUTOCTRL_REG_AUTOPD_LEN       1
#define FS16XX_AUTOCTRL_REG_AUTOPD_MSK       0x100
#define FS16XX_AUTOCTRL_REG_SILEDTEN         (0x01<<9)
#define FS16XX_AUTOCTRL_REG_SILEDTEN_POS     9
#define FS16XX_AUTOCTRL_REG_SILEDTEN_LEN     1
#define FS16XX_AUTOCTRL_REG_SILEDTEN_MSK     0x200
#define FS16XX_AUTOCTRL_REG_SILECOND         (0x01<<10)
#define FS16XX_AUTOCTRL_REG_SILECOND_POS     10
#define FS16XX_AUTOCTRL_REG_SILECOND_LEN     3
#define FS16XX_AUTOCTRL_REG_SILECOND_MSK     0x1c00
#define FS16XX_AUTOCTRL_REG_SILESTA          (0x01<<15)
#define FS16XX_AUTOCTRL_REG_SILESTA_POS      15
#define FS16XX_AUTOCTRL_REG_SILESTA_LEN      1
#define FS16XX_AUTOCTRL_REG_SILESTA_MSK      0x8000

/*************** OTPCMD (0x98) ***************/
#define FS16XX_OTPCMD_REG                0x98

#define FS16XX_OTPCMD_REG_R                  (0x01<<0)
#define FS16XX_OTPCMD_REG_R_POS              0
#define FS16XX_OTPCMD_REG_R_LEN              1
#define FS16XX_OTPCMD_REG_R_MSK              0x1
#define FS16XX_OTPCMD_REG_W                  (0x01<<1)
#define FS16XX_OTPCMD_REG_W_POS              1
#define FS16XX_OTPCMD_REG_W_LEN              1
#define FS16XX_OTPCMD_REG_W_MSK              0x2
#define FS16XX_OTPCMD_REG_BUSY               (0x01<<2)
#define FS16XX_OTPCMD_REG_BUSY_POS           2
#define FS16XX_OTPCMD_REG_BUSY_LEN           1
#define FS16XX_OTPCMD_REG_BUSY_MSK           0x4

/*************** OTPADDR (0x99) ***************/
#define FS16XX_OTPADDR_REG                0x99

#define FS16XX_OTPADDR_REG_ADDR              (0x01<<0)
#define FS16XX_OTPADDR_REG_ADDR_POS          0
#define FS16XX_OTPADDR_REG_ADDR_LEN          5
#define FS16XX_OTPADDR_REG_ADDR_MSK          0x1f

/*************** OTPWDATA (0x9A) ***************/
#define FS16XX_OTPWDATA_REG                0x9A

#define FS16XX_OTPWDATA_REG_WDATA            (0x01<<0)
#define FS16XX_OTPWDATA_REG_WDATA_POS        0
#define FS16XX_OTPWDATA_REG_WDATA_LEN        8
#define FS16XX_OTPWDATA_REG_WDATA_MSK        0xff

/*************** OTPRDATA (0x9B) ***************/
#define FS16XX_OTPRDATA_REG                0x9B

#define FS16XX_OTPRDATA_REG_RDATA_L          (0x01<<0)
#define FS16XX_OTPRDATA_REG_RDATA_L_POS      0
#define FS16XX_OTPRDATA_REG_RDATA_L_LEN      8
#define FS16XX_OTPRDATA_REG_RDATA_L_MSK      0xff
#define FS16XX_OTPRDATA_REG_RDATA_H          (0x01<<8)
#define FS16XX_OTPRDATA_REG_RDATA_H_POS      8
#define FS16XX_OTPRDATA_REG_RDATA_H_LEN      8
#define FS16XX_OTPRDATA_REG_RDATA_H_MSK      0xff00

/*************** I2SSET (0xA0) ***************/
#define FS16XX_I2SSET_REG                0xA0

#define FS16XX_I2SSET_REG_BCMP               (0x01<<3)
#define FS16XX_I2SSET_REG_BCMP_POS           3
#define FS16XX_I2SSET_REG_BCMP_LEN           2
#define FS16XX_I2SSET_REG_BCMP_MSK           0x18
#define FS16XX_I2SSET_REG_LRCLKP             (0x01<<5)
#define FS16XX_I2SSET_REG_LRCLKP_POS         5
#define FS16XX_I2SSET_REG_LRCLKP_LEN         1
#define FS16XX_I2SSET_REG_LRCLKP_MSK         0x20
#define FS16XX_I2SSET_REG_BCLKP              (0x01<<6)
#define FS16XX_I2SSET_REG_BCLKP_POS          6
#define FS16XX_I2SSET_REG_BCLKP_LEN          1
#define FS16XX_I2SSET_REG_BCLKP_MSK          0x40
#define FS16XX_I2SSET_REG_I2SOSWAP           (0x01<<7)
#define FS16XX_I2SSET_REG_I2SOSWAP_POS       7
#define FS16XX_I2SSET_REG_I2SOSWAP_LEN       1
#define FS16XX_I2SSET_REG_I2SOSWAP_MSK       0x80
#define FS16XX_I2SSET_REG_AECSEL             (0x01<<8)
#define FS16XX_I2SSET_REG_AECSEL_POS         8
#define FS16XX_I2SSET_REG_AECSEL_LEN         3
#define FS16XX_I2SSET_REG_AECSEL_MSK         0x700
#define FS16XX_I2SSET_REG_BCLKSTA            (0x01<<15)
#define FS16XX_I2SSET_REG_BCLKSTA_POS        15
#define FS16XX_I2SSET_REG_BCLKSTA_LEN        1
#define FS16XX_I2SSET_REG_BCLKSTA_MSK        0x8000

/*************** DSPCTRL (0xA1) ***************/
#define FS16XX_DSPCTRL_REG                0xA1

#define FS16XX_DSPCTRL_REG_DCCOEF            (0x01<<0)
#define FS16XX_DSPCTRL_REG_DCCOEF_POS        0
#define FS16XX_DSPCTRL_REG_DCCOEF_LEN        3
#define FS16XX_DSPCTRL_REG_DCCOEF_MSK        0x7
#define FS16XX_DSPCTRL_REG_DCBP              (0x01<<3)
#define FS16XX_DSPCTRL_REG_DCBP_POS          3
#define FS16XX_DSPCTRL_REG_DCBP_LEN          1
#define FS16XX_DSPCTRL_REG_DCBP_MSK          0x8
#define FS16XX_DSPCTRL_REG_NOFILTEN          (0x01<<4)
#define FS16XX_DSPCTRL_REG_NOFILTEN_POS      4
#define FS16XX_DSPCTRL_REG_NOFILTEN_LEN      1
#define FS16XX_DSPCTRL_REG_NOFILTEN_MSK      0x10
#define FS16XX_DSPCTRL_REG_COEFSEL           (0x01<<6)
#define FS16XX_DSPCTRL_REG_COEFSEL_POS       6
#define FS16XX_DSPCTRL_REG_COEFSEL_LEN       1
#define FS16XX_DSPCTRL_REG_COEFSEL_MSK       0x40
#define FS16XX_DSPCTRL_REG_ACSEQEN           (0x01<<7)
#define FS16XX_DSPCTRL_REG_ACSEQEN_POS       7
#define FS16XX_DSPCTRL_REG_ACSEQEN_LEN       1
#define FS16XX_DSPCTRL_REG_ACSEQEN_MSK       0x80
#define FS16XX_DSPCTRL_REG_EQBEN             (0x01<<10)
#define FS16XX_DSPCTRL_REG_EQBEN_POS         10
#define FS16XX_DSPCTRL_REG_EQBEN_LEN         1
#define FS16XX_DSPCTRL_REG_EQBEN_MSK         0x400
#define FS16XX_DSPCTRL_REG_EQEN              (0x01<<11)
#define FS16XX_DSPCTRL_REG_EQEN_POS          11
#define FS16XX_DSPCTRL_REG_EQEN_LEN          1
#define FS16XX_DSPCTRL_REG_EQEN_MSK          0x800
#define FS16XX_DSPCTRL_REG_DSPEN             (0x01<<12)
#define FS16XX_DSPCTRL_REG_DSPEN_POS         12
#define FS16XX_DSPCTRL_REG_DSPEN_LEN         1
#define FS16XX_DSPCTRL_REG_DSPEN_MSK         0x1000

/*************** DACEQWL (0xA2) ***************/
#define FS16XX_DACEQWL_REG                0xA2
/*************** DACEQWH (0xA3) ***************/
#define FS16XX_DACEQWH_REG                0xA3
/*************** DACEQA (0xA6) ***************/
#define FS16XX_DACEQA_REG                0xA6

#define FS16XX_DACEQA_REG_ADDR               (0x01<<0)
#define FS16XX_DACEQA_REG_ADDR_POS           0
#define FS16XX_DACEQA_REG_ADDR_LEN           8
#define FS16XX_DACEQA_REG_ADDR_MSK           0xff

/*************** AGCCTRL (0xA7) ***************/
#define FS16XX_AGCCTRL_REG                0xA7

#define FS16XX_AGCCTRL_REG_EXP_EN            (0x01<<0)
#define FS16XX_AGCCTRL_REG_EXP_EN_POS        0
#define FS16XX_AGCCTRL_REG_EXP_EN_LEN        1
#define FS16XX_AGCCTRL_REG_EXP_EN_MSK        0x1
#define FS16XX_AGCCTRL_REG_LIMIT_EN          (0x01<<1)
#define FS16XX_AGCCTRL_REG_LIMIT_EN_POS      1
#define FS16XX_AGCCTRL_REG_LIMIT_EN_LEN      1
#define FS16XX_AGCCTRL_REG_LIMIT_EN_MSK      0x2
#define FS16XX_AGCCTRL_REG_EN                (0x01<<2)
#define FS16XX_AGCCTRL_REG_EN_POS            2
#define FS16XX_AGCCTRL_REG_EN_LEN            1
#define FS16XX_AGCCTRL_REG_EN_MSK            0x4

/*************** AGCTHD (0xA8) ***************/
#define FS16XX_AGCTHD_REG                0xA8

#define FS16XX_AGCTHD_REG_LM_THRD            (0x01<<0)
#define FS16XX_AGCTHD_REG_LM_THRD_POS        0
#define FS16XX_AGCTHD_REG_LM_THRD_LEN        8
#define FS16XX_AGCTHD_REG_LM_THRD_MSK        0xff
#define FS16XX_AGCTHD_REG_EXP_THRD           (0x01<<8)
#define FS16XX_AGCTHD_REG_EXP_THRD_POS       8
#define FS16XX_AGCTHD_REG_EXP_THRD_LEN       8
#define FS16XX_AGCTHD_REG_EXP_THRD_MSK       0xff00

/*************** AGCPARA (0xA9) ***************/
#define FS16XX_AGCPARA_REG                0xA9

#define FS16XX_AGCPARA_REG_EXPD_ES           (0x01<<0)
#define FS16XX_AGCPARA_REG_EXPD_ES_POS       0
#define FS16XX_AGCPARA_REG_EXPD_ES_LEN       3
#define FS16XX_AGCPARA_REG_EXPD_ES_MSK       0x7
#define FS16XX_AGCPARA_REG_REL               (0x01<<3)
#define FS16XX_AGCPARA_REG_REL_POS           3
#define FS16XX_AGCPARA_REG_REL_LEN           13
#define FS16XX_AGCPARA_REG_REL_MSK           0xfff8

/*************** AGCMG (0xAA) ***************/
#define FS16XX_AGCMG_REG                0xAA

#define FS16XX_AGCMG_REG_GAIN                (0x01<<0)
#define FS16XX_AGCMG_REG_GAIN_POS            0
#define FS16XX_AGCMG_REG_GAIN_LEN            8
#define FS16XX_AGCMG_REG_GAIN_MSK            0xff

/*************** DRCCTRL (0xAB) ***************/
#define FS16XX_DRCCTRL_REG                0xAB

#define FS16XX_DRCCTRL_REG_EXP_EN            (0x01<<0)
#define FS16XX_DRCCTRL_REG_EXP_EN_POS        0
#define FS16XX_DRCCTRL_REG_EXP_EN_LEN        1
#define FS16XX_DRCCTRL_REG_EXP_EN_MSK        0x1
#define FS16XX_DRCCTRL_REG_LIMIT_EN          (0x01<<1)
#define FS16XX_DRCCTRL_REG_LIMIT_EN_POS      1
#define FS16XX_DRCCTRL_REG_LIMIT_EN_LEN      1
#define FS16XX_DRCCTRL_REG_LIMIT_EN_MSK      0x2
#define FS16XX_DRCCTRL_REG_EN                (0x01<<2)
#define FS16XX_DRCCTRL_REG_EN_POS            2
#define FS16XX_DRCCTRL_REG_EN_LEN            1
#define FS16XX_DRCCTRL_REG_EN_MSK            0x4

/*************** DRCTHD (0xAC) ***************/
#define FS16XX_DRCTHD_REG                0xAC

#define FS16XX_DRCTHD_REG_LM_THRD            (0x01<<0)
#define FS16XX_DRCTHD_REG_LM_THRD_POS        0
#define FS16XX_DRCTHD_REG_LM_THRD_LEN        8
#define FS16XX_DRCTHD_REG_LM_THRD_MSK        0xff
#define FS16XX_DRCTHD_REG_EXP_THRD           (0x01<<8)
#define FS16XX_DRCTHD_REG_EXP_THRD_POS       8
#define FS16XX_DRCTHD_REG_EXP_THRD_LEN       8
#define FS16XX_DRCTHD_REG_EXP_THRD_MSK       0xff00

/*************** DRCPARA (0xAD) ***************/
#define FS16XX_DRCPARA_REG                0xAD

#define FS16XX_DRCPARA_REG_EXPAND_ES         (0x01<<0)
#define FS16XX_DRCPARA_REG_EXPAND_ES_POS     0
#define FS16XX_DRCPARA_REG_EXPAND_ES_LEN     3
#define FS16XX_DRCPARA_REG_EXPAND_ES_MSK     0x7
#define FS16XX_DRCPARA_REG_REL               (0x01<<3)
#define FS16XX_DRCPARA_REG_REL_POS           3
#define FS16XX_DRCPARA_REG_REL_LEN           13
#define FS16XX_DRCPARA_REG_REL_MSK           0xfff8

/*************** DACCTRL (0xAE) ***************/
#define FS16XX_DACCTRL_REG                0xAE

#define FS16XX_DACCTRL_REG_SDMSTLBYP         (0x01<<4)
#define FS16XX_DACCTRL_REG_SDMSTLBYP_POS     4
#define FS16XX_DACCTRL_REG_SDMSTLBYP_LEN     1
#define FS16XX_DACCTRL_REG_SDMSTLBYP_MSK     0x10
#define FS16XX_DACCTRL_REG_MUTE              (0x01<<8)
#define FS16XX_DACCTRL_REG_MUTE_POS          8
#define FS16XX_DACCTRL_REG_MUTE_LEN          1
#define FS16XX_DACCTRL_REG_MUTE_MSK          0x100
#define FS16XX_DACCTRL_REG_FADE              (0x01<<9)
#define FS16XX_DACCTRL_REG_FADE_POS          9
#define FS16XX_DACCTRL_REG_FADE_LEN          1
#define FS16XX_DACCTRL_REG_FADE_MSK          0x200

/*************** TSCTRL (0xAF) ***************/
#define FS16XX_TSCTRL_REG                0xAF

#define FS16XX_TSCTRL_REG_GAIN               (0x01<<0)
#define FS16XX_TSCTRL_REG_GAIN_POS           0
#define FS16XX_TSCTRL_REG_GAIN_LEN           3
#define FS16XX_TSCTRL_REG_GAIN_MSK           0x7
#define FS16XX_TSCTRL_REG_EN                 (0x01<<3)
#define FS16XX_TSCTRL_REG_EN_POS             3
#define FS16XX_TSCTRL_REG_EN_LEN             1
#define FS16XX_TSCTRL_REG_EN_MSK             0x8
#define FS16XX_TSCTRL_REG_OFF_THD            (0x01<<4)
#define FS16XX_TSCTRL_REG_OFF_THD_POS        4
#define FS16XX_TSCTRL_REG_OFF_THD_LEN        3
#define FS16XX_TSCTRL_REG_OFF_THD_MSK        0x70
#define FS16XX_TSCTRL_REG_OFF_DELAY          (0x01<<8)
#define FS16XX_TSCTRL_REG_OFF_DELAY_POS      8
#define FS16XX_TSCTRL_REG_OFF_DELAY_LEN      3
#define FS16XX_TSCTRL_REG_OFF_DELAY_MSK      0x700
#define FS16XX_TSCTRL_REG_OFF_ZEROCRS        (0x01<<12)
#define FS16XX_TSCTRL_REG_OFF_ZEROCRS_POS    12
#define FS16XX_TSCTRL_REG_OFF_ZEROCRS_LEN    1
#define FS16XX_TSCTRL_REG_OFF_ZEROCRS_MSK    0x1000
#define FS16XX_TSCTRL_REG_OFF_AUTOEN         (0x01<<13)
#define FS16XX_TSCTRL_REG_OFF_AUTOEN_POS     13
#define FS16XX_TSCTRL_REG_OFF_AUTOEN_LEN     1
#define FS16XX_TSCTRL_REG_OFF_AUTOEN_MSK     0x2000
#define FS16XX_TSCTRL_REG_OFFSTA             (0x01<<14)
#define FS16XX_TSCTRL_REG_OFFSTA_POS         14
#define FS16XX_TSCTRL_REG_OFFSTA_LEN         1
#define FS16XX_TSCTRL_REG_OFFSTA_MSK         0x4000

/*************** MODCTRL (0xB0) ***************/
#define FS16XX_MODCTRL_REG                0xB0

#define FS16XX_MODCTRL_REG_G1_SEL            (0x01<<0)
#define FS16XX_MODCTRL_REG_G1_SEL_POS        0
#define FS16XX_MODCTRL_REG_G1_SEL_LEN        3
#define FS16XX_MODCTRL_REG_G1_SEL_MSK        0x7
#define FS16XX_MODCTRL_REG_G2_SEL            (0x01<<3)
#define FS16XX_MODCTRL_REG_G2_SEL_POS        3
#define FS16XX_MODCTRL_REG_G2_SEL_LEN        3
#define FS16XX_MODCTRL_REG_G2_SEL_MSK        0x38
#define FS16XX_MODCTRL_REG_DEMSEL            (0x01<<6)
#define FS16XX_MODCTRL_REG_DEMSEL_POS        6
#define FS16XX_MODCTRL_REG_DEMSEL_LEN        1
#define FS16XX_MODCTRL_REG_DEMSEL_MSK        0x40
#define FS16XX_MODCTRL_REG_DEMBYP            (0x01<<7)
#define FS16XX_MODCTRL_REG_DEMBYP_POS        7
#define FS16XX_MODCTRL_REG_DEMBYP_LEN        1
#define FS16XX_MODCTRL_REG_DEMBYP_MSK        0x80
#define FS16XX_MODCTRL_REG_DITHPOS           (0x01<<8)
#define FS16XX_MODCTRL_REG_DITHPOS_POS       8
#define FS16XX_MODCTRL_REG_DITHPOS_LEN       5
#define FS16XX_MODCTRL_REG_DITHPOS_MSK       0x1f00
#define FS16XX_MODCTRL_REG_DITHRANGE         (0x01<<13)
#define FS16XX_MODCTRL_REG_DITHRANGE_POS     13
#define FS16XX_MODCTRL_REG_DITHRANGE_LEN     1
#define FS16XX_MODCTRL_REG_DITHRANGE_MSK     0x2000
#define FS16XX_MODCTRL_REG_DITHCLR           (0x01<<14)
#define FS16XX_MODCTRL_REG_DITHCLR_POS       14
#define FS16XX_MODCTRL_REG_DITHCLR_LEN       1
#define FS16XX_MODCTRL_REG_DITHCLR_MSK       0x4000
#define FS16XX_MODCTRL_REG_DITHEN            (0x01<<15)
#define FS16XX_MODCTRL_REG_DITHEN_POS        15
#define FS16XX_MODCTRL_REG_DITHEN_LEN        1
#define FS16XX_MODCTRL_REG_DITHEN_MSK        0x8000

/*************** DTINI (0xB1) ***************/
#define FS16XX_DTINI_REG                0xB1

#define FS16XX_DTINI_REG_INI_VAL             (0x01<<0)
#define FS16XX_DTINI_REG_INI_VAL_POS         0
#define FS16XX_DTINI_REG_INI_VAL_LEN         16
#define FS16XX_DTINI_REG_INI_VAL_MSK         0xffff

/*************** DTFD (0xB2) ***************/
#define FS16XX_DTFD_REG                0xB2

#define FS16XX_DTFD_REG_COEFF                (0x01<<0)
#define FS16XX_DTFD_REG_COEFF_POS            0
#define FS16XX_DTFD_REG_COEFF_LEN            16
#define FS16XX_DTFD_REG_COEFF_MSK            0xffff

/*************** ADCCTRL (0xB3) ***************/
#define FS16XX_ADCCTRL_REG                0xB3

#define FS16XX_ADCCTRL_REG_EQB1EN_R          (0x01<<8)
#define FS16XX_ADCCTRL_REG_EQB1EN_R_POS      8
#define FS16XX_ADCCTRL_REG_EQB1EN_R_LEN      1
#define FS16XX_ADCCTRL_REG_EQB1EN_R_MSK      0x100
#define FS16XX_ADCCTRL_REG_EQB0EN_R          (0x01<<9)
#define FS16XX_ADCCTRL_REG_EQB0EN_R_POS      9
#define FS16XX_ADCCTRL_REG_EQB0EN_R_LEN      1
#define FS16XX_ADCCTRL_REG_EQB0EN_R_MSK      0x200
#define FS16XX_ADCCTRL_REG_ADCREN            (0x01<<12)
#define FS16XX_ADCCTRL_REG_ADCREN_POS        12
#define FS16XX_ADCCTRL_REG_ADCREN_LEN        1
#define FS16XX_ADCCTRL_REG_ADCREN_MSK        0x1000
#define FS16XX_ADCCTRL_REG_ADCRSEL           (0x01<<13)
#define FS16XX_ADCCTRL_REG_ADCRSEL_POS       13
#define FS16XX_ADCCTRL_REG_ADCRSEL_LEN       1
#define FS16XX_ADCCTRL_REG_ADCRSEL_MSK       0x2000

/*************** ADCEQWL (0xB4) ***************/
#define FS16XX_ADCEQWL_REG                0xB4

/*************** ADCEQWH (0xB5) ***************/
#define FS16XX_ADCEQWH_REG                0xB5

/*************** ADCEQRL (0xB6) ***************/
#define FS16XX_ADCEQRL_REG                0xB6

/*************** ADCEQRH (0xB7) ***************/
#define FS16XX_ADCEQRH_REG                0xB7

/*************** ADCEQA (0xB8) ***************/
#define FS16XX_ADCEQA_REG                0xB8

/*************** ADCENV (0xB9) ***************/
#define FS16XX_ADCENV_REG                0xB9

#define FS16XX_ADCENV_REG_AMP_DT_A           (0x01<<0)
#define FS16XX_ADCENV_REG_AMP_DT_A_POS       0
#define FS16XX_ADCENV_REG_AMP_DT_A_LEN       13
#define FS16XX_ADCENV_REG_AMP_DT_A_MSK       0x1fff
#define FS16XX_ADCENV_REG_AVG_NUM            (0x01<<13)
#define FS16XX_ADCENV_REG_AVG_NUM_POS        13
#define FS16XX_ADCENV_REG_AVG_NUM_LEN        2
#define FS16XX_ADCENV_REG_AVG_NUM_MSK        0x6000
#define FS16XX_ADCENV_REG_AMP_DT_EN          (0x01<<15)
#define FS16XX_ADCENV_REG_AMP_DT_EN_POS      15
#define FS16XX_ADCENV_REG_AMP_DT_EN_LEN      1
#define FS16XX_ADCENV_REG_AMP_DT_EN_MSK      0x8000

/*************** ADCTIME (0xBA) ***************/
#define FS16XX_ADCTIME_REG                0xBA

#define FS16XX_ADCTIME_REG_STABLE_TM         (0x01<<0)
#define FS16XX_ADCTIME_REG_STABLE_TM_POS     0
#define FS16XX_ADCTIME_REG_STABLE_TM_LEN     3
#define FS16XX_ADCTIME_REG_STABLE_TM_MSK     0x7
#define FS16XX_ADCTIME_REG_CHK_TM            (0x01<<3)
#define FS16XX_ADCTIME_REG_CHK_TM_POS        3
#define FS16XX_ADCTIME_REG_CHK_TM_LEN        3
#define FS16XX_ADCTIME_REG_CHK_TM_MSK        0x38

/*************** ZMDATA (0xBB) ***************/
#define FS16XX_ZMDATA_REG                0xBB


/*************** DACENV (0xBC) ***************/
#define FS16XX_DACENV_REG                0xBC


/*************** DIGSTAT (0xBD) ***************/
#define FS16XX_DIGSTAT_REG                0xBD

#define FS16XX_DIGSTAT_REG_ADCRUN            (0x01<<0)
#define FS16XX_DIGSTAT_REG_ADCRUN_POS        0
#define FS16XX_DIGSTAT_REG_ADCRUN_LEN        1
#define FS16XX_DIGSTAT_REG_ADCRUN_MSK        0x1
#define FS16XX_DIGSTAT_REG_DACRUN            (0x01<<1)
#define FS16XX_DIGSTAT_REG_DACRUN_POS        1
#define FS16XX_DIGSTAT_REG_DACRUN_LEN        1
#define FS16XX_DIGSTAT_REG_DACRUN_MSK        0x2
#define FS16XX_DIGSTAT_REG_SPKM24S           (0x01<<4)
#define FS16XX_DIGSTAT_REG_SPKM24S_POS       4
#define FS16XX_DIGSTAT_REG_SPKM24S_LEN       1
#define FS16XX_DIGSTAT_REG_SPKM24S_MSK       0x10
#define FS16XX_DIGSTAT_REG_SPKM6S            (0x01<<5)
#define FS16XX_DIGSTAT_REG_SPKM6S_POS        5
#define FS16XX_DIGSTAT_REG_SPKM6S_LEN        1
#define FS16XX_DIGSTAT_REG_SPKM6S_MSK        0x20
#define FS16XX_DIGSTAT_REG_SPKRES            (0x01<<6)
#define FS16XX_DIGSTAT_REG_SPKRES_POS        6
#define FS16XX_DIGSTAT_REG_SPKRES_LEN        1
#define FS16XX_DIGSTAT_REG_SPKRES_MSK        0x40
#define FS16XX_DIGSTAT_REG_SPKFSM            (0x01<<12)
#define FS16XX_DIGSTAT_REG_SPKFSM_POS        12
#define FS16XX_DIGSTAT_REG_SPKFSM_LEN        4
#define FS16XX_DIGSTAT_REG_SPKFSM_MSK        0xf000

/*************** I2SPINC (0xBE) ***************/
#define FS16XX_I2SPINC_REG                0xBE

#define FS16XX_I2SPINC_REG_BCPDD             (0x01<<0)
#define FS16XX_I2SPINC_REG_BCPDD_POS         0
#define FS16XX_I2SPINC_REG_BCPDD_LEN         1
#define FS16XX_I2SPINC_REG_BCPDD_MSK         0x1
#define FS16XX_I2SPINC_REG_LRPDD             (0x01<<1)
#define FS16XX_I2SPINC_REG_LRPDD_POS         1
#define FS16XX_I2SPINC_REG_LRPDD_LEN         1
#define FS16XX_I2SPINC_REG_LRPDD_MSK         0x2
#define FS16XX_I2SPINC_REG_SDOPDD            (0x01<<2)
#define FS16XX_I2SPINC_REG_SDOPDD_POS        2
#define FS16XX_I2SPINC_REG_SDOPDD_LEN        1
#define FS16XX_I2SPINC_REG_SDOPDD_MSK        0x4
#define FS16XX_I2SPINC_REG_SDIPDD            (0x01<<3)
#define FS16XX_I2SPINC_REG_SDIPDD_POS        3
#define FS16XX_I2SPINC_REG_SDIPDD_LEN        1
#define FS16XX_I2SPINC_REG_SDIPDD_MSK        0x8
#define FS16XX_I2SPINC_REG_MCLKPDD           (0x01<<4)
#define FS16XX_I2SPINC_REG_MCLKPDD_POS       4
#define FS16XX_I2SPINC_REG_MCLKPDD_LEN       1
#define FS16XX_I2SPINC_REG_MCLKPDD_MSK       0x10

/*************** ADPBST (0xBF) ***************/
#define FS16XX_ADPBST_REG                0xBF

/*************** BSTCTRL (0xC0) ***************/
#define FS16XX_BSTCTRL_REG                0xC0

#define FS16XX_BSTCTRL_REG_EPROM_LD          (0x01<<0)
#define FS16XX_BSTCTRL_REG_EPROM_LD_POS      0
#define FS16XX_BSTCTRL_REG_EPROM_LD_LEN      1
#define FS16XX_BSTCTRL_REG_EPROM_LD_MSK      0x1
#define FS16XX_BSTCTRL_REG_DISCHARGE         (0x01<<1)
#define FS16XX_BSTCTRL_REG_DISCHARGE_POS     1
#define FS16XX_BSTCTRL_REG_DISCHARGE_LEN     1
#define FS16XX_BSTCTRL_REG_DISCHARGE_MSK     0x2
#define FS16XX_BSTCTRL_REG_DAC_GAIN          (0x01<<2)
#define FS16XX_BSTCTRL_REG_DAC_GAIN_POS      2
#define FS16XX_BSTCTRL_REG_DAC_GAIN_LEN      2
#define FS16XX_BSTCTRL_REG_DAC_GAIN_MSK      0xc
#define FS16XX_BSTCTRL_REG_BSTEN             (0x01<<4)
#define FS16XX_BSTCTRL_REG_BSTEN_POS         4
#define FS16XX_BSTCTRL_REG_BSTEN_LEN         1
#define FS16XX_BSTCTRL_REG_BSTEN_MSK         0x10
#define FS16XX_BSTCTRL_REG_MODE_CTRL         (0x01<<5)
#define FS16XX_BSTCTRL_REG_MODE_CTRL_POS     5
#define FS16XX_BSTCTRL_REG_MODE_CTRL_LEN     1
#define FS16XX_BSTCTRL_REG_MODE_CTRL_MSK     0x20
#define FS16XX_BSTCTRL_REG_ILIM_SEL          (0x01<<6)
#define FS16XX_BSTCTRL_REG_ILIM_SEL_POS      6
#define FS16XX_BSTCTRL_REG_ILIM_SEL_LEN      4
#define FS16XX_BSTCTRL_REG_ILIM_SEL_MSK      0x3c0
#define FS16XX_BSTCTRL_REG_VOUT_SEL          (0x01<<10)
#define FS16XX_BSTCTRL_REG_VOUT_SEL_POS      10
#define FS16XX_BSTCTRL_REG_VOUT_SEL_LEN      5
#define FS16XX_BSTCTRL_REG_VOUT_SEL_MSK      0x7c00
#define FS16XX_BSTCTRL_REG_SSEND             (0x01<<15)
#define FS16XX_BSTCTRL_REG_SSEND_POS         15
#define FS16XX_BSTCTRL_REG_SSEND_LEN         1
#define FS16XX_BSTCTRL_REG_SSEND_MSK         0x8000


/*************** PLLCTRL4 (0xC4) ***************/
#define FS16XX_PLLCTRL4_REG                0xC4

#define FS16XX_PLLCTRL4_REG_PLLEN            (0x01<<0)
#define FS16XX_PLLCTRL4_REG_PLLEN_POS        0
#define FS16XX_PLLCTRL4_REG_PLLEN_LEN        1
#define FS16XX_PLLCTRL4_REG_PLLEN_MSK        0x1
#define FS16XX_PLLCTRL4_REG_OSCEN            (0x01<<1)
#define FS16XX_PLLCTRL4_REG_OSCEN_POS        1
#define FS16XX_PLLCTRL4_REG_OSCEN_LEN        1
#define FS16XX_PLLCTRL4_REG_OSCEN_MSK        0x2
#define FS16XX_PLLCTRL4_REG_ZMEN             (0x01<<2)
#define FS16XX_PLLCTRL4_REG_ZMEN_POS         2
#define FS16XX_PLLCTRL4_REG_ZMEN_LEN         1
#define FS16XX_PLLCTRL4_REG_ZMEN_MSK         0x4
#define FS16XX_PLLCTRL4_REG_VBGEN            (0x01<<3)
#define FS16XX_PLLCTRL4_REG_VBGEN_POS        3
#define FS16XX_PLLCTRL4_REG_VBGEN_LEN        1
#define FS16XX_PLLCTRL4_REG_VBGEN_MSK        0x8

/*************** OCCTRL (0xC5) ***************/
#define FS16XX_OCCTRL_REG                0xC5

#define FS16XX_OCCTRL_REG_OCNUM              (0x01<<0)
#define FS16XX_OCCTRL_REG_OCNUM_POS          0
#define FS16XX_OCCTRL_REG_OCNUM_LEN          8
#define FS16XX_OCCTRL_REG_OCNUM_MSK          0xff

/*************** OTCTRL (0xC6) ***************/
#define FS16XX_OTCTRL_REG                0xC6

#define FS16XX_OTCTRL_REG_OTTHD_L            (0x01<<0)
#define FS16XX_OTCTRL_REG_OTTHD_L_POS        0
#define FS16XX_OTCTRL_REG_OTTHD_L_LEN        8
#define FS16XX_OTCTRL_REG_OTTHD_L_MSK        0xff
#define FS16XX_OTCTRL_REG_OTTHD_H            (0x01<<8)
#define FS16XX_OTCTRL_REG_OTTHD_H_POS        8
#define FS16XX_OTCTRL_REG_OTTHD_H_LEN        8
#define FS16XX_OTCTRL_REG_OTTHD_H_MSK        0xff00

/*************** UVCTRL (0xC7) ***************/
#define FS16XX_UVCTRL_REG                0xC7

#define FS16XX_UVCTRL_REG_UVTHD_L            (0x01<<0)
#define FS16XX_UVCTRL_REG_UVTHD_L_POS        0
#define FS16XX_UVCTRL_REG_UVTHD_L_LEN        8
#define FS16XX_UVCTRL_REG_UVTHD_L_MSK        0xff
#define FS16XX_UVCTRL_REG_UVTHD_H            (0x01<<8)
#define FS16XX_UVCTRL_REG_UVTHD_H_POS        8
#define FS16XX_UVCTRL_REG_UVTHD_H_LEN        8
#define FS16XX_UVCTRL_REG_UVTHD_H_MSK        0xff00

/*************** OVCTRL (0xC8) ***************/
#define FS16XX_OVCTRL_REG                0xC8

#define FS16XX_OVCTRL_REG_OVTHD_L            (0x01<<0)
#define FS16XX_OVCTRL_REG_OVTHD_L_POS        0
#define FS16XX_OVCTRL_REG_OVTHD_L_LEN        8
#define FS16XX_OVCTRL_REG_OVTHD_L_MSK        0xff
#define FS16XX_OVCTRL_REG_OVTHD_H            (0x01<<8)
#define FS16XX_OVCTRL_REG_OVTHD_H_POS        8
#define FS16XX_OVCTRL_REG_OVTHD_H_LEN        8
#define FS16XX_OVCTRL_REG_OVTHD_H_MSK        0xff00

/*************** SPKERR (0xC9) ***************/
#define FS16XX_SPKERR_REG                0xC9

#define FS16XX_SPKERR_REG_THRD               (0x01<<0)
#define FS16XX_SPKERR_REG_THRD_POS           0
#define FS16XX_SPKERR_REG_THRD_LEN           16
#define FS16XX_SPKERR_REG_THRD_MSK           0xffff

/*************** SPKM24 (0xCA) ***************/
#define FS16XX_SPKM24_REG                0xCA

#define FS16XX_SPKM24_REG_THRD               (0x01<<0)
#define FS16XX_SPKM24_REG_THRD_POS           0
#define FS16XX_SPKM24_REG_THRD_LEN           16
#define FS16XX_SPKM24_REG_THRD_MSK           0xffff

/*************** SPKM6 (0xCB) ***************/
#define FS16XX_SPKM6_REG                0xCB

#define FS16XX_SPKM6_REG_THRD                (0x01<<0)
#define FS16XX_SPKM6_REG_THRD_POS            0
#define FS16XX_SPKM6_REG_THRD_LEN            16
#define FS16XX_SPKM6_REG_THRD_MSK            0xffff

/*************** SPKRE (0xCC) ***************/
#define FS16XX_SPKRE_REG                0xCC

#define FS16XX_SPKRE_REG_THRD                (0x01<<0)
#define FS16XX_SPKRE_REG_THRD_POS            0
#define FS16XX_SPKRE_REG_THRD_LEN            16
#define FS16XX_SPKRE_REG_THRD_MSK            0xffff

/*************** DACPARA1 (0xCD) ***************/
#define FS16XX_DACPARA1_REG                0xCD

/*************** DACPARA2 (0xCE) ***************/
#define FS16XX_DACPARA2_REG                0xCE

/*************** ANACTRL (0xD0) ***************/
#define FS16XX_ANACTRL_REG                0xD0

#define FS16XX_ANACTRL_REG_BPCLKCK           (0x01<<0)
#define FS16XX_ANACTRL_REG_BPCLKCK_POS       0
#define FS16XX_ANACTRL_REG_BPCLKCK_LEN       1
#define FS16XX_ANACTRL_REG_BPCLKCK_MSK       0x1
#define FS16XX_ANACTRL_REG_BPUVOV            (0x01<<1)
#define FS16XX_ANACTRL_REG_BPUVOV_POS        1
#define FS16XX_ANACTRL_REG_BPUVOV_LEN        1
#define FS16XX_ANACTRL_REG_BPUVOV_MSK        0x2
#define FS16XX_ANACTRL_REG_BPOT              (0x01<<2)
#define FS16XX_ANACTRL_REG_BPOT_POS          2
#define FS16XX_ANACTRL_REG_BPOT_LEN          1
#define FS16XX_ANACTRL_REG_BPOT_MSK          0x4
#define FS16XX_ANACTRL_REG_BPOC              (0x01<<3)
#define FS16XX_ANACTRL_REG_BPOC_POS          3
#define FS16XX_ANACTRL_REG_BPOC_LEN          1
#define FS16XX_ANACTRL_REG_BPOC_MSK          0x8
#define FS16XX_ANACTRL_REG_BPOV              (0x01<<4)
#define FS16XX_ANACTRL_REG_BPOV_POS          4
#define FS16XX_ANACTRL_REG_BPOV_LEN          1
#define FS16XX_ANACTRL_REG_BPOV_MSK          0x10
#define FS16XX_ANACTRL_REG_BPSPKOT           (0x01<<5)
#define FS16XX_ANACTRL_REG_BPSPKOT_POS       5
#define FS16XX_ANACTRL_REG_BPSPKOT_LEN       1
#define FS16XX_ANACTRL_REG_BPSPKOT_MSK       0x20
#define FS16XX_ANACTRL_REG_PTSEQBP           (0x01<<6)
#define FS16XX_ANACTRL_REG_PTSEQBP_POS       6
#define FS16XX_ANACTRL_REG_PTSEQBP_LEN       1
#define FS16XX_ANACTRL_REG_PTSEQBP_MSK       0x40
#define FS16XX_ANACTRL_REG_PW                (0x01<<7)
#define FS16XX_ANACTRL_REG_PW_POS            7
#define FS16XX_ANACTRL_REG_PW_LEN            1
#define FS16XX_ANACTRL_REG_PW_MSK            0x80


/*************** ZMCONFIG (0xD5) ***************/
#define FS16XX_ZMCONFIG_REG                0xD5

#define FS16XX_ZMCONFIG_REG_CALIBEN          (0x01<<4)
#define FS16XX_ZMCONFIG_REG_CALIBEN_POS      4
#define FS16XX_ZMCONFIG_REG_CALIBEN_LEN      1
#define FS16XX_ZMCONFIG_REG_CALIBEN_MSK      0x10
#define FS16XX_ZMCONFIG_REG_SELFO            (0x01<<5)
#define FS16XX_ZMCONFIG_REG_SELFO_POS        5
#define FS16XX_ZMCONFIG_REG_SELFO_LEN        1
#define FS16XX_ZMCONFIG_REG_SELFO_MSK        0x20

/*************** I2CCTRL (0xD9) ***************/
#define FS16XX_I2CCTRL_REG                0xD9

#define FS16XX_I2CCTRL_REG_TIMEOUTEN         (0x01<<0)
#define FS16XX_I2CCTRL_REG_TIMEOUTEN_POS     0
#define FS16XX_I2CCTRL_REG_TIMEOUTEN_LEN     1
#define FS16XX_I2CCTRL_REG_TIMEOUTEN_MSK     0x1
#define FS16XX_I2CCTRL_REG_BPDEGLITCH        (0x01<<1)
#define FS16XX_I2CCTRL_REG_BPDEGLITCH_POS    1
#define FS16XX_I2CCTRL_REG_BPDEGLITCH_LEN    1
#define FS16XX_I2CCTRL_REG_BPDEGLITCH_MSK    0x2

/*************** OTPPG0W0 (0xE0) ***************/
#define FS16XX_OTPPG0W0_REG                0xE0

/*************** OTPPG0W1 (0xE1) ***************/
#define FS16XX_OTPPG0W1_REG                0xE1

/*************** OTPPG0W2 (0xE2) ***************/
#define FS16XX_OTPPG0W2_REG                0xE2

/*************** OTPPG0W3 (0xE3) ***************/
#define FS16XX_OTPPG0W3_REG                0xE3

/*************** OTPPG1W0 (0xE4) ***************/
#define FS16XX_OTPPG1W0_REG                0xE4

/*************** OTPPG1W1 (0xE5) ***************/
#define FS16XX_OTPPG1W1_REG                0xE5

/*************** OTPPG1W2 (0xE6) ***************/
#define FS16XX_OTPPG1W2_REG                0xE6

/*************** OTPPG1W3 (0xE7) ***************/
#define FS16XX_OTPPG1W3_REG                0xE7

/*************** OTPPG2 (0xE8) ***************/
#define FS16XX_OTPPG2_REG                0xE8

#endif /* Generated at: 2017-08-31.11:41:09 */
