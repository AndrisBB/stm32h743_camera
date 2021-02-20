#include "ov7670.h"

#include <stdint.h>
#include <stdio.h>

#define OV7670_REG_GAIN                     0x00
#define OV7670_REG_BLUE                     0x01
#define OV7670_REG_RED                      0x02
#define OV7670_REG_VREF                     0x03
#define OV7670_REG_COM1                     0x04
#define OV7670_REG_BAVE                     0x05
#define OV7670_REG_GBAVE                    0x06
#define OV7670_REG_AECHH                    0x07
#define OV7670_REG_RAVE                     0x08
#define OV7670_REG_COM2                     0x09
#define OV7670_REG_PID                      0x0A
#define OV7670_REG_VER                      0x0B
#define OV7670_REG_COM3                     0x0C
    #define COM3_ENABLE_SCALE               0x08
#define OV7670_REG_COM4                     0x0D
#define OV7670_REG_COM5                     0x0E
#define OV7670_REG_COM6                     0x0F
#define OV7670_REG_AECH                     0x10
#define OV7670_REG_CLKRC                    0x11
    #define CLKRC_DISABLE_PRESCALER         0x40
#define OV7670_REG_COM7                     0x12
    #define COM7_ENABLE_CIF                 0x20
    #define COM7_ENABLE_QVGA                0x10
    #define COM7_ENABLE_QCIF                0x08
#define OV7670_REG_COM8                     0x13
#define OV7670_REG_COM9                     0x14
#define OV7670_REG_COM10                    0x15
#define OV7670_REG_HSTART                   0x17
#define OV7670_REG_HSTOP                    0x18
#define OV7670_REG_VSTRT                    0x19
#define OV7670_REG_VSTOP                    0x1A
#define OV7670_REG_PSHFT                    0x1B
#define OV7670_REG_MIDH                     0x1C
#define OV7670_REG_MIDL                     0x1D
#define OV7670_REG_MVFP                     0x1E
#define OV7670_REG_LAEC                     0x1F
#define OV7670_REG_ADCCTR0                  0x20
#define OV7670_REG_ADCCTR1                  0x21
#define OV7670_REG_ADCCTR2                  0x22
#define OV7670_REG_ADCCTR3                  0x23
#define OV7670_REG_AEW                      0x24
#define OV7670_REG_AEB                      0x25
#define OV7670_REG_VPT                      0x26
#define OV7670_REG_BBIAS                    0x27
#define OV7670_REG_GBBIAS                   0x28
#define OV7670_REG_EXHCH                    0x2A
#define OV7670_REG_EXHCL                    0x2B
#define OV7670_REG_RBIAS                    0x2C
#define OV7670_REG_ADVFL                    0x2D
#define OV7670_REG_ADVFH                    0x2E
#define OV7670_REG_YAVE                     0x2F
#define OV7670_REG_HSYST                    0x30
#define OV7670_REG_HSYEN                    0x31
#define OV7670_REG_HREF                     0x32
#define OV7670_REG_CHLF                     0x33
#define OV7670_REG_ARBLM                    0x34
#define OV7670_REG_ADC                      0x37
#define OV7670_REG_ACOM                     0x38
#define OV7670_REG_OFON                     0x39
#define OV7670_REG_TSLB                     0x3A
#define OV7670_REG_COM11                    0x3B
#define OV7670_REG_COM12                    0x3C
#define OV7670_REG_COM13                    0x3D
#define OV7670_REG_COM14                    0x3E
#define OV7670_REG_EDGE                     0x3F
#define OV7670_REG_COM15                    0x40
#define OV7670_REG_COM16                    0x41
#define OV7670_REG_COM17                    0x42
#define OV7670_REG_AWBC1                    0x43
#define OV7670_REG_AWBC2                    0x44
#define OV7670_REG_AWBC3                    0x45
#define OV7670_REG_AWBC4                    0x46
#define OV7670_REG_AWBC5                    0x47
#define OV7670_REG_AWBC6                    0x48
#define OV7670_REG_REG4B                    0x4B
#define OV7670_REG_DNSTH                    0x4C
#define OV7670_REG_MTX1                     0x4F
#define OV7670_REG_MTX2                     0x50
#define OV7670_REG_MTX3                     0x51
#define OV7670_REG_MTX4                     0x52
#define OV7670_REG_MTX5                     0x53
#define OV7670_REG_MTX6                     0x54
#define OV7670_REG_BRIGHT                   0x55
#define OV7670_REG_CONTRAS                  0x56
#define OV7670_REG_CONTRAS_CENTER           0x57
#define OV7670_REG_MTXS                     0x58
#define OV7670_REG_LCC1                     0x62
#define OV7670_REG_LCC2                     0x63
#define OV7670_REG_LCC3                     0x64
#define OV7670_REG_LCC4                     0x65
#define OV7670_REG_LCC5                     0x66
#define OV7670_REG_MANU                     0x67
#define OV7670_REG_MANV                     0x68
#define OV7670_REG_GFIX                     0x69
#define OV7670_REG_GGAIN                    0x6A
#define OV7670_REG_DBLV                     0x6B
#define OV7670_REG_AWBCTR3                  0x6C
#define OV7670_REG_AWBCTR2                  0x6D
#define OV7670_REG_AWBCTR1                  0x6E
#define OV7670_REG_AWBCTR0                  0x6F
#define OV7670_REG_SCALING_XSC              0x70
#define OV7670_REG_SCALING_YSC              0x71
#define OV7670_REG_SCALING_DCWCTR           0x72
#define OV7670_REG_SCALING_PCLK_DIV         0x73
#define OV7670_REG_REG74                    0x74
#define OV7670_REG_REG75                    0x75
#define OV7670_REG_REG76                    0x76
#define OV7670_REG_REG77                    0x77
#define OV7670_REG_SLOP                     0x7A
#define OV7670_REG_GAM1                     0x7B
#define OV7670_REG_GAM2                     0x7C
#define OV7670_REG_GAM3                     0x7D
#define OV7670_REG_GAM4                     0x7E
#define OV7670_REG_GAM5                     0x7F
#define OV7670_REG_GAM6                     0x80
#define OV7670_REG_GAM7                     0x81
#define OV7670_REG_GAM8                     0x82
#define OV7670_REG_GAM9                     0x83
#define OV7670_REG_GAM10                    0x84
#define OV7670_REG_GAM11                    0x85
#define OV7670_REG_GAM12                    0x86
#define OV7670_REG_GAM13                    0x87
#define OV7670_REG_GAM14                    0x88
#define OV7670_REG_GAM15                    0x89
#define OV7670_REG_RGB444                   0x8C
#define OV7670_REG_DM_LNL                   0x92
#define OV7670_REG_DM_LNH                   0x93
#define OV7670_REG_LCC6                     0x94
#define OV7670_REG_LCC7                     0x95
#define OV7670_REG_BD50ST                   0x9D
#define OV7670_REG_BD60ST                   0x9E
#define OV7670_REG_HAECC1                   0x9F
#define OV7670_REG_HAECC2                   0xA0
#define OV7670_REG_SCALING_PCLK_DELAY       0xA2
#define OV7670_REG_NT_CTRL                  0xA4
#define OV7670_REG_BD50MAX                  0xA5
#define OV7670_REG_HAECC3                   0xA6
#define OV7670_REG_HAECC4                   0xA7
#define OV7670_REG_HAECC5                   0xA8
#define OV7670_REG_HAECC6                   0xA9
#define OV7670_REG_HAECC7                   0xAA
#define OV7670_REG_BD60MAX                  0xAB
#define OV7670_REG_STR_OPT                  0xAC
#define OV7670_REG_STR_R                    0xAD
#define OV7670_REG_STR_G                    0xAE
#define OV7670_REG_STR_B                    0xAF
#define OV7670_REG_ABLC1                    0xB1
#define OV7670_REG_THL_ST                   0xB3
#define OV7670_REG_THL_DLT                  0xB5
#define OV7670_REG_AD_CHB                   0xBE
#define OV7670_REG_AD_CHR                   0xBF
#define OV7670_REG_AD_CHGB                  0xC0
#define OV7670_REG_AD_CHGR                  0xC1
#define OV7670_REG_SATCTR                   0xC9

#define OV7670_WRITE_ADDR                   0x42
#define OV7670_READ_ADDR                    0x43

#define OV7670_MAX_FPS                      30
#define OV7670_MHZ_PER_FRAME                800000
#define OV7670_MAX_CLK_PRESCALE             32

// #define OV7670_DEBUG_DUMP_ALL               0


#ifdef OV7670_DEBUG_DUMP_ALL
    static uint8_t dump_reg_list[] = {
        OV7670_REG_GAIN,    OV7670_REG_BLUE,    OV7670_REG_RED,     OV7670_REG_VREF,    OV7670_REG_COM1,    OV7670_REG_BAVE,    OV7670_REG_GBAVE,   OV7670_REG_AECHH,
        OV7670_REG_RAVE,    OV7670_REG_COM2,    OV7670_REG_PID,     OV7670_REG_VER,     OV7670_REG_COM3,    OV7670_REG_COM4,    OV7670_REG_COM5,    OV7670_REG_COM6,
        OV7670_REG_AECH,    OV7670_REG_CLKRC,   OV7670_REG_COM7,    OV7670_REG_COM8,    OV7670_REG_COM9,    OV7670_REG_COM10,   OV7670_REG_HSTART,  OV7670_REG_HSTOP,
        OV7670_REG_VSTRT,   OV7670_REG_VSTOP,   OV7670_REG_PSHFT,   OV7670_REG_MIDH,    OV7670_REG_MIDL,    OV7670_REG_MVFP,    OV7670_REG_LAEC,    OV7670_REG_ADCCTR0,
        OV7670_REG_ADCCTR1, OV7670_REG_ADCCTR2, OV7670_REG_ADCCTR3, OV7670_REG_AEW,     OV7670_REG_AEB,     OV7670_REG_VPT,     OV7670_REG_BBIAS,   OV7670_REG_GBBIAS,
        OV7670_REG_EXHCH,   OV7670_REG_EXHCL,   OV7670_REG_RBIAS,   OV7670_REG_ADVFL,   OV7670_REG_ADVFH,   OV7670_REG_YAVE,    OV7670_REG_HSYST,   OV7670_REG_HSYEN,
        OV7670_REG_HREF,    OV7670_REG_CHLF,    OV7670_REG_ARBLM,   OV7670_REG_ADC,     OV7670_REG_ACOM,    OV7670_REG_OFON,    OV7670_REG_TSLB,    OV7670_REG_COM11,
        OV7670_REG_COM12,   OV7670_REG_COM13,   OV7670_REG_COM14,   OV7670_REG_EDGE,    OV7670_REG_COM15,   OV7670_REG_COM16,   OV7670_REG_COM17,   OV7670_REG_AWBC1,
        OV7670_REG_AWBC2,   OV7670_REG_AWBC3,   OV7670_REG_AWBC4,   OV7670_REG_AWBC5,   OV7670_REG_AWBC6,   OV7670_REG_REG4B,   OV7670_REG_DNSTH,   OV7670_REG_MTX1,
        OV7670_REG_MTX2,    OV7670_REG_MTX3,    OV7670_REG_MTX4,    OV7670_REG_MTX5,    OV7670_REG_MTX6,    OV7670_REG_BRIGHT,  OV7670_REG_CONTRAS, OV7670_REG_CONTRAS_CENTER,
        OV7670_REG_MTXS,    OV7670_REG_LCC1,    OV7670_REG_LCC2,    OV7670_REG_LCC3,    OV7670_REG_LCC4,    OV7670_REG_LCC5,    OV7670_REG_MANU,    OV7670_REG_MANV,
        OV7670_REG_GFIX,    OV7670_REG_GGAIN,   OV7670_REG_DBLV,    OV7670_REG_AWBCTR3, OV7670_REG_AWBCTR2, OV7670_REG_AWBCTR1, OV7670_REG_AWBCTR0, OV7670_REG_SCALING_XSC,
        OV7670_REG_SCALING_YSC,OV7670_REG_SCALING_DCWCTR,OV7670_REG_SCALING_PCLK_DIV,OV7670_REG_REG74,OV7670_REG_REG75,OV7670_REG_REG76,OV7670_REG_REG77,OV7670_REG_SLOP,
        OV7670_REG_GAM1,    OV7670_REG_GAM2,    OV7670_REG_GAM3,    OV7670_REG_GAM4,    OV7670_REG_GAM5,    OV7670_REG_GAM6,    OV7670_REG_GAM7,    OV7670_REG_GAM8,
        OV7670_REG_GAM9,    OV7670_REG_GAM10,   OV7670_REG_GAM11,   OV7670_REG_GAM12,   OV7670_REG_GAM13,   OV7670_REG_GAM14,   OV7670_REG_GAM15,   OV7670_REG_RGB444,
        OV7670_REG_DM_LNL,  OV7670_REG_DM_LNH,  OV7670_REG_LCC6,    OV7670_REG_LCC7,    OV7670_REG_BD50ST,  OV7670_REG_BD60ST,  OV7670_REG_HAECC1,  OV7670_REG_HAECC2,
        OV7670_REG_SCALING_PCLK_DELAY,OV7670_REG_NT_CTRL,OV7670_REG_BD50MAX, OV7670_REG_HAECC3,OV7670_REG_HAECC4,OV7670_REG_HAECC5,OV7670_REG_HAECC6,OV7670_REG_HAECC7,
        OV7670_REG_BD60MAX, OV7670_REG_STR_OPT, OV7670_REG_STR_R,   OV7670_REG_STR_G,   OV7670_REG_STR_B,   OV7670_REG_ABLC1,   OV7670_REG_THL_ST,  OV7670_REG_THL_DLT,
        OV7670_REG_AD_CHB,  OV7670_REG_AD_CHR,  OV7670_REG_AD_CHGB, OV7670_REG_AD_CHGR, OV7670_REG_SATCTR
    };
#else
    static uint8_t dump_reg_list[] = {
        OV7670_REG_CLKRC, OV7670_REG_DBLV, OV7670_REG_COM3, OV7670_REG_COM7
    };
#endif
    
#define OV7670_DEFAULT_QCIF_1FPS            1

#ifdef OV7670_DEFAULT_QCIF_1FPS
    static uint8_t default_register_values[] = 
    {
        OV7670_REG_CLKRC,     ((30 >> 1) - 1)               // Divide input clk by 10 (8mhz/10 = 800khz = 1fps)
        // ,OV7670_REG_COM3,      COM3_ENABLE_SCALE  ,
        // OV7670_REG_COM7,      COM7_ENABLE_QVGA,
        // OV7670_REG_SCALING_XSC, 0x3A,
        // ,OV7670_REG_SCALING_YSC, 0xC1
    };
#else
    static uint8_t default_register_values[] = {
        OV7670_REG_CLKRC,     ((10 >> 1) - 1)               // Divide input clk by 10 (8mhz/10 = 800khz = 1fps)
    };
#endif

static uint8_t      ov7670_read_reg(I2C_HandleTypeDef *hi2c1, uint8_t register_address);
static void         ov7670_write_reg(I2C_HandleTypeDef *hi2c1, uint8_t register_address, uint8_t value);

// static void DMA_HalfTransferM0(DMA_HandleTypeDef *hdma)
// {
//     // printf("DMA_HalfTransferM0 \r\n");
//     return;
// }

// static void Memory0_completed(DMA_HandleTypeDef *hdma)
// {
//     printf("Memory0\r\n");

//     uint8_t *buffer;
//     DMA_Stream_TypeDef  *stream = hdma->Instance;

//     if(bp_acuire(&pool, &buffer) != 0) {
//         printf("Failed to acuire buffer, stop DMA\r\n");
//         stream->CR &= ~DMA_SxCR_EN;
//     }
//     else {
//         printf("buffer address:%p\r\n", buffer);
//         stream->M0AR = (uint32_t)buffer;
//     }
// }

// static void Memory1_completed(DMA_HandleTypeDef *hdma)
// {
//     printf("Memory1\r\n");

//     uint8_t *buffer;
//     DMA_Stream_TypeDef  *stream = hdma->Instance;

//     if(bp_acuire(&pool, &buffer) != 0) {
//         printf("Failed to acuire buffer, stop DMA\r\n");
//         stream->CR &= ~DMA_SxCR_EN;
//     }
//     else {
//         printf("buffer address:%p\r\n", buffer);
//         stream->M1AR = (uint32_t)buffer;
//     }
// }

OV7670_status_t ov7670_init(ov7670_handle_t *handle)
{
    uint8_t i = 0;

    // Write default register values
    for(i = 0; i < sizeof(default_register_values)/sizeof(default_register_values[0]); i=i+2) {
        ov7670_write_reg(handle->hi2c, default_register_values[i], default_register_values[i+1]);
    }

    HAL_Delay(100);
    ov7670_dump_registers(handle);

    return OV7670_OK;
}

// void ov7670_snapshot(DCMI_HandleTypeDef *hdcmi)
// {
//     printf("%s: called\r\n", __func__);

//     // Enable DCMI bit 14
//     hdcmi->Instance->CR |= 0x4000;

//     // Clear CAPTURE/SNAPHOT BIT6
//     hdcmi->Instance->CR |= 0x002;
//     hdcmi->Instance->IER |= 0x1;

//     hdcmi->DMA_Handle->XferCpltCallback = Memory0_completed;
//     hdcmi->DMA_Handle->XferM1CpltCallback = Memory1_completed;

//     // Setup DMA manually

//     uint8_t *ptr_0, *ptr_1;

//     if(bp_acuire(&pool, &ptr_0) != 0) {
//         printf("Failed to allocato buffer 0\r\n");
//         return;
//     }

//     if(bp_acuire(&pool, &ptr_1) != 0) {
//         printf("Failed to allocato buffer 1\r\n");
//         return;
//     }

//     printf("ptr_0:%p, ptr_1:%p\r\n", ptr_0, ptr_1);

//     DMA_Stream_TypeDef  *stream = hdcmi->DMA_Handle->Instance;
//     stream->CR |= DMA_SxCR_DBM;
//     stream->PAR =  (uint32_t)&hdcmi->Instance->DR;
//     stream->M0AR = (uint32_t)ptr_0;
//     stream->M1AR = (uint32_t)ptr_1;
//     stream->NDTR = (uint32_t)pool.item_size / 4;

//     uint32_t *ifcRegister_Base = (uint32_t *)((uint32_t)(hdcmi->DMA_Handle->StreamBaseAddress + 8U));
//     *ifcRegister_Base = 0x3FUL << (hdcmi->DMA_Handle->StreamIndex & 0x1FU);

//     stream->CR  |= DMA_SxCR_TCIE | DMA_SxCR_TEIE;    // Enable transfer complete
//     stream->FCR |= 0x00000080U;

//     stream->CR |= DMA_SxCR_EN;      // Start stream

//     hdcmi->Instance->CR |= 0x1;

//     return;
// }

// void ov7670_dump_framebuffer()
// {
//     uint8_t *ptr = fb_memory;

//     // for(int i = 0; i < 8; i++) {
//     //     if(HAL_UART_Transmit(&huart2, ptr,  WIDTH * HEIGHT * BYTES_PER_PIXEL / 8, HAL_MAX_DELAY) != HAL_OK)
//     //     {
//     //         printf("Failed to dump frame 0\r\n");
//     //     }
//     //     ptr += WIDTH * HEIGHT * BYTES_PER_PIXEL / 8;
//     // }

//     uint8_t data[4];

//     for(int i = 0; i < WIDTH * HEIGHT * BYTES_PER_PIXEL; i = i + 4) {
//         data[0] = ptr[i + 3];
//         data[1] = ptr[i + 2];
//         data[2] = ptr[i + 1];
//         data[3] = ptr[i + 0];

//         // data[0] = ptr[i + 0];
//         // data[1] = ptr[i + 1];
//         // data[2] = ptr[i + 2];
//         // data[3] = ptr[i + 3];

//         if(HAL_UART_Transmit(&huart2, data, 4, HAL_MAX_DELAY) != HAL_OK)
//         {
//             printf("Failed to dump frame 0\r\n");
//         }
//     }
// }

void ov7670_dump_registers(ov7670_handle_t *handle)
{
    uint8_t value;

    printf("\r\n  Register values\r\n");
    printf("------------------------------------\r\n");
    
    for(uint8_t i = 0; i < sizeof(dump_reg_list); i++) {
        value = ov7670_read_reg(handle->hi2c, dump_reg_list[i]);
        printf("    0x%02X:0x%02X\r\n", dump_reg_list[i], value);
    }

     printf("\r\n");
}

uint8_t ov7670_read_reg(I2C_HandleTypeDef *hi2c1, uint8_t register_address)
{
    uint8_t value = 0xFF;

    if(HAL_I2C_Master_Transmit(hi2c1, OV7670_WRITE_ADDR, &register_address, 1, 1000) == HAL_OK) {
        HAL_I2C_Master_Receive(hi2c1, OV7670_READ_ADDR, &value, 1, 1000);
    }
    else {
        printf("Failed to read register 0x%02X\r\n", register_address);
    }

    return value;
}

void ov7670_write_reg(I2C_HandleTypeDef *hi2c1, uint8_t register_address, uint8_t value)
{
    uint8_t data[2];

    data[0] = register_address;
    data[1] = value;  

    printf("Write register 0x%02X with value 0x%02X\r\n", data[0], data[1]);

    if(HAL_I2C_Master_Transmit(hi2c1, OV7670_WRITE_ADDR, data, 2, 1000) != HAL_OK) {
        printf("Failed to write register 0x%02X with value 0x%02X\r\n", data[0], data[1]);
    }
}
