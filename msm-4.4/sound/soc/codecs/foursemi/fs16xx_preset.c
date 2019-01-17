/* 
* Copyright (C) 2016 Fourier Semiconductor Inc. 
*
*/
#include "fs16xx.h"
#include "fs16xx_regs.h"
#include "fs16xx_preset.h"

static int preset_init = 0;
static Fs16xx_preset gFs16xxPresets[(int)fs16xx_preset_type_max];

#ifndef FS_STEREO_MODE 
static char const *gFs16xxPresetName[(int)fs16xx_preset_type_max] = {
    "fs16xx_01s_mono.preset",
    ""
};
#else
static char const *gFs16xxPresetName[(int)fs16xx_preset_type_max] = {
    "fs16xx_01s_left.preset",
    "fs16xx_01s_right.preset"
};
#endif

#define CRC16_TABLE_SIZE 256
const unsigned short gCrc16Polynomial = 0xA001;
static unsigned short gCrc16Table[CRC16_TABLE_SIZE];

#define PRESET_VALID    1
#define PRESET_INVALID    0

/* Initialize presets should only be called once in fs16xx_init
*/
Fs16xx_Error_t fs16xx_initialize_presets(void) {
    unsigned short value;
    unsigned short temp, i;
    unsigned char j;
    if(preset_init) return Fs16xx_Error_OK;
    memset(gFs16xxPresets, 0, sizeof(Fs16xx_preset) * (int)fs16xx_preset_type_max);

    // Initialize CRC16 table
    for (i = 0; i < CRC16_TABLE_SIZE; ++i)
    {
        value = 0;
        temp = i;
        for (j = 0; j < 8; ++j)
        {
            if (((value ^ temp) & 0x0001) != 0)
            {
                value = (unsigned short)((value >> 1) ^ gCrc16Polynomial);
            }
            else
            {
                value >>= 1;
            }
            temp >>= 1;
        }
        gCrc16Table[i] = value;
    }
    preset_init = 1;
    return Fs16xx_Error_OK;
}

Fs16xx_Error_t fs16xx_deinitialize_presets(void) {
    int i;
    for(i = 0; i < (int)fs16xx_preset_type_max; i++) {
        if(PRESET_VALID == gFs16xxPresets[i].valid) {
            if(gFs16xxPresets[i].preset_header.data) {
                gFs16xxPresets[i].valid = PRESET_INVALID;
                kfree(gFs16xxPresets[i].preset_header.data);
            }
        }
    }

    return Fs16xx_Error_OK;
}

// load_presets should only be called once after initialization
Fs16xx_Error_t fs16xx_load_presets(Fs16xx_devId_t id) {
    const struct firmware *fs_firmware = NULL;
    struct snd_soc_codec *codec;
    int data_len, buf_size, ret;
    unsigned short *ptr_data;
    unsigned short checksum;

    DEBUGPRINT("%s enter. id = %d", __func__, id);

    if(FS16XX_DEV_INDEX_L != id && FS16XX_DEV_INDEX_R != id) {
        PRINT_ERROR("%s invalid id[%d]", __func__, id);
        return Fs16xx_Error_Bad_Parameter;
    }

    codec = fs16xx_get_codec_by_id(id);
    if(NULL == codec) {
        PRINT_ERROR("%s invalid codec with id[%d]", __func__, id);
        return Fs16xx_Error_Bad_Parameter;
    }

    fs16xx_initialize_presets();

    if(gFs16xxPresets[id].valid) {
        PRINT_ERROR("%s invalid status, presets already loaded.", __func__);
        return Fs16xx_Error_StateInvalid;
    }

    ret = request_firmware(&fs_firmware, gFs16xxPresetName[id], codec->dev);
    if (ret) {
        PRINT_ERROR("%s Failed to load fs preset table %s", __func__, gFs16xxPresetName[id]);
        return Fs16xx_Error_Invalid_Preset;
    }
    DEBUGPRINT("loaded firmware size = 0x%04zx", fs_firmware->size);
    if(fs_firmware->size < sizeof(Fs16xx_preset_header_t)) {
        PRINT_ERROR("%s Invalid preset table %s size=%zd", __func__,
            gFs16xxPresetName[id], fs_firmware->size);
        release_firmware(fs_firmware);
        return Fs16xx_Error_Invalid_Preset;
    }
    
    // Parse header
    memcpy(&(gFs16xxPresets[id].preset_header), fs_firmware->data, FS_PRESET_HEADER_LEN);

    // Checking preset version
    if(PRESET_VERSION != gFs16xxPresets[id].preset_header.preset_ver) {
        PRINT_ERROR("%s preset version[0x%04X] invalid!",
            __func__, gFs16xxPresets[id].preset_header.preset_ver);
        release_firmware(fs_firmware);
        return Fs16xx_Error_Invalid_Preset;
    }
    data_len = gFs16xxPresets[id].preset_header.data_len;
    if(data_len <= 0 || (data_len % 2) != 0) {
        PRINT_ERROR("%s preset header [%s] validation checking failed. data_len=%d", 
            __func__, gFs16xxPresetName[id], data_len);
        release_firmware(fs_firmware);
        
    }

    buf_size = data_len * sizeof(unsigned short);
    ptr_data = kzalloc(buf_size, GFP_KERNEL);
    if (!ptr_data) {
        PRINT_ERROR("%s failed to allocate %d bytes.", __func__, buf_size);
        gFs16xxPresets[id].preset_header.data = 0;
        release_firmware(fs_firmware);
        return Fs16xx_Error_Invalid_Preset;
    }

    memcpy(ptr_data, ((u8*)fs_firmware->data) + FS_PRESET_HEADER_LEN, buf_size);
    release_firmware(fs_firmware);
    checksum = fs16xx_calc_checksum(ptr_data, data_len);
    if(checksum != gFs16xxPresets[id].preset_header.crc16) {
        PRINT_ERROR("%s preset data [%s] checksum not match. checksum=0x%04X", 
            __func__, gFs16xxPresetName[id], checksum);
        gFs16xxPresets[id].preset_header.data = 0;
        kfree(ptr_data);
        return Fs16xx_Error_Invalid_Preset;
    }

    gFs16xxPresets[id].preset_header.data = ptr_data;
    gFs16xxPresets[id].valid = PRESET_VALID;
    
    fs16xx_load_presets_to_dev(id);
    DEBUGPRINT("%s successfully exit.", __func__);
    return Fs16xx_Error_OK;
}

Fs16xx_Error_t fs16xx_load_presets_to_dev(Fs16xx_devId_t id) {
    int i, data_len;
    Fs16xx_Error_t err = Fs16xx_Error_OK;
    unsigned short *ptr_data = 0;
    //unsigned char dataBuf[4];
    unsigned short valPwr;
    Fs_Dev_State *dev_state = fs16xx_get_dev_state_by_id(id);

    if(FS16XX_DEV_INDEX_L != id && FS16XX_DEV_INDEX_R != id) {
        PRINT_ERROR("%s invalid id[%d]", __func__, id);
        return Fs16xx_Error_Bad_Parameter;
    }
    if(dev_state == NULL){
        PRINT_ERROR("%s dev_state is null!", __func__);
        return Fs16xx_Error_Bad_Parameter;
    }

    if(gFs16xxPresets[id].valid != PRESET_VALID 
        || gFs16xxPresets[id].preset_header.data == NULL) {
        PRINT_ERROR("%s invalid preset id[%d] data=%p", __func__, 
            id, gFs16xxPresets[id].preset_header.data);
        return Fs16xx_Error_Invalid_Preset;
    }

    dev_state->cur_preset = -1; // Reset preset index
    dev_state->tmax = gFs16xxPresets[id].preset_header.tmax;
    dev_state->tempr_coef = gFs16xxPresets[id].preset_header.tempr_coef;
    dev_state->tempr_sel = gFs16xxPresets[id].preset_header.tempr_sel;
    dev_state->spk_set = gFs16xxPresets[id].preset_header.spk_set;
    dev_state->adp_bst = gFs16xxPresets[id].preset_header.adp_bst[0];
    dev_state->bst_ctrl = gFs16xxPresets[id].preset_header.bst_ctrl[0];
    dev_state->ts_ctrl = gFs16xxPresets[id].preset_header.ts_ctrl[0];

    data_len = gFs16xxPresets[id].preset_header.data_len;
    ptr_data = gFs16xxPresets[id].preset_header.data;
    dev_state->dev_preset = &(gFs16xxPresets[id]);
    
    DEBUGPRINT("%s ptr_data=%p data_len=%d", __func__, ptr_data, data_len);

    // Amp off
    err |= fs16xx_read_register16(id, FS16XX_SYSCTRL_REG, &valPwr);
    err |= fs16xx_write_register16(id, FS16XX_SYSCTRL_REG, valPwr & (~FS16XX_SYSCTRL_REG_AMPE));

    // Set spk_set
    err |= fs16xx_write_register16(id, FS16XX_SPKSET_REG, gFs16xxPresets[id].preset_header.spk_set);
    // Set tempr coef to TEMPSEL, refer to Bug 55
    err |= fs16xx_write_register16(id, FS16XX_TEMPSEL_REG, (gFs16xxPresets[id].preset_header.tempr_coef) << 1);

    fs16xx_write_register16(id, FS16XX_OTPACC_REG, FS16XX_OTP_ACC_KEY2);
    // Start from position 0
    err |= fs16xx_write_register16(id, FS16XX_DACEQA_REG, 0);

    if(Fs16xx_Error_OK == fs16xx_wait_dsp_off(id, 0, 1000)) {
        for (i = 0; i < data_len; i += 2) {
            err |= fs16xx_write_register16(id, FS16XX_DACEQWL_REG, ptr_data[i]);
            err |= fs16xx_write_register16(id, FS16XX_DACEQWH_REG, ptr_data[i + 1]);
            //dataBuf[0] = ((ptr_data[i]) >> 8) & 0xFF;
            //dataBuf[1] = (ptr_data[i]) & 0xFF;
            //dataBuf[2] = ((ptr_data[i + 1]) >> 8) & 0xFF;
            //dataBuf[3] = (ptr_data[i + 1]) & 0xFF;
            //err |= fs16xx_bulkwrite_register(id, FS16XX_DACEQWL_REG, sizeof(dataBuf), dataBuf);
        }
    } else {
        PRINT_ERROR("%s failed to wait for dsp off!", __func__);
        return Fs16xx_Error_StateInvalid;
    }

    err |= fs16xx_write_register16(id, FS16XX_OTPACC_REG, 0);
    // Recover amp
    err |= fs16xx_write_register16(id, FS16XX_SYSCTRL_REG, valPwr);

    return err;
}

unsigned short fs16xx_calc_checksum(unsigned short *data, int len) {
    unsigned short crc = 0;
    unsigned char b, index;
    int i;
    if(len <= 0) return 0;
    
    for (i = 0; i < len; i++)
    {
        b = (unsigned char)(data[i] & 0xFF);
        index = (unsigned char)(crc ^ b);
        crc = (unsigned short)((crc >> 8) ^ gCrc16Table[index]);

        b = (unsigned char)((data[i] >> 8) & 0xFF);
        index = (unsigned char)(crc ^ b);
        crc = (unsigned short)((crc >> 8) ^ gCrc16Table[index]);
    }
    return crc;
}

/* fs16xx_set_preset must be called when osc is on */
Fs16xx_Error_t fs16xx_set_preset(Fs16xx_devId_t id, int preset) {
    Fs16xx_Error_t err = Fs16xx_Error_OK;
    unsigned short valPwr, valDspCtrl, valPll;
    int i;
    Fs_Dev_State *dev_state = NULL;
    Fs16xx_preset_header_t *preset_header = NULL;

    PRINT_ERROR("%s set preset %d.", __func__, preset);

    if (Fs16xx_Error_OK != fs16xx_handle_is_open(id)) {
        PRINT_ERROR("%s device not opened.", __func__);
        return Fs16xx_Error_NotOpen;
    }

    dev_state = fs16xx_get_dev_state_by_id(id);
    if(!dev_state) {
        PRINT_ERROR("%s invalid id, exit!", __func__);
        return Fs16xx_Error_StateInvalid;
    }

    if(!dev_state->dev_preset) {
        PRINT_ERROR("%s invalid dev_preset, exit!", __func__);
        return Fs16xx_Error_StateInvalid;
    }

    if(preset != 0 && preset != 1) {
        PRINT_ERROR("%s invalid preset value! preset = %d", __func__, preset);
        return Fs16xx_Error_Bad_Parameter;
    }

    if(preset == dev_state->cur_preset) {
        DEBUGPRINT("%s cur preset already set %d, exit.", __func__, preset);
        return Fs16xx_Error_OK;
    }
    i = preset;
    preset_header = &(dev_state->dev_preset->preset_header);

    if(!dev_state->dev_preset->valid || preset_header == NULL) {
        PRINT_ERROR("%s preset invalid! preset = %d preset_header = %p", __func__, preset, preset_header);
        return Fs16xx_Error_Bad_Parameter;
    }

    // Amp off
    err |= fs16xx_read_register16(id, FS16XX_SYSCTRL_REG, &valPwr);
    err |= fs16xx_write_register16(id, FS16XX_SYSCTRL_REG, valPwr & (~FS16XX_SYSCTRL_REG_AMPE));

    if(Fs16xx_Error_OK == fs16xx_wait_dsp_off(id, 0, 50)) {
        // Enable osc
        err |= fs16xx_read_register16(id, FS16XX_PLLCTRL4_REG, &valPll);
        err |= fs16xx_write_register16(id, FS16XX_PLLCTRL4_REG, valPll | FS16XX_PLLCTRL4_REG_OSCEN);

        dev_state->adp_bst = preset_header->adp_bst[i];
        dev_state->bst_ctrl = preset_header->bst_ctrl[i];
        // TODO: TS control is not set to register
        dev_state->ts_ctrl = preset_header->ts_ctrl[i];

        err |= fs16xx_write_register16(id, FS16XX_AGCCTRL_REG, preset_header->agc_ctrl[i]);
        err |= fs16xx_write_register16(id, FS16XX_AGCTHD_REG, preset_header->agc_thd[i]);
        err |= fs16xx_write_register16(id, FS16XX_AGCPARA_REG, preset_header->agc_para[i]);
        err |= fs16xx_write_register16(id, FS16XX_AGCMG_REG, preset_header->agc_mg[i]);
        err |= fs16xx_write_register16(id, FS16XX_DACPARA1_REG, preset_header->dac_para1[i]);
        err |= fs16xx_write_register16(id, FS16XX_ADPBST_REG, preset_header->adp_bst[i]);
        err |= fs16xx_write_register16(id, FS16XX_BSTCTRL_REG, preset_header->bst_ctrl[i]);

        if(preset == 0) {
            valDspCtrl = (unsigned short)(FS16XX_DSP_ENABLE_ALL_MASK & (~FS16XX_DSPCTRL_REG_COEFSEL_MSK));
        } else {
            valDspCtrl = (unsigned short)(FS16XX_DSP_ENABLE_ALL_MASK | FS16XX_DSPCTRL_REG_COEFSEL_MSK);
        }

        err |= fs16xx_write_register16(id, FS16XX_DSPCTRL_REG, valDspCtrl);
        dev_state->cur_preset = (int)preset;

        err |= fs16xx_write_register16(id, FS16XX_PLLCTRL4_REG, valPll);
    } else {
        PRINT_ERROR("%s failed to wait for dsp off!", __func__);
    }

    err |= fs16xx_write_register16(id, FS16XX_SYSCTRL_REG, valPwr);

    DEBUGPRINT("%s exit err=%d.", __func__, err);
    return err;
}

