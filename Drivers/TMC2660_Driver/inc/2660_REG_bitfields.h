#ifndef TMC_2660_REG_BITFIELDS_H_
#define TMC_2660_REG_BITFIELDS_H_
#include "Types.h"

typedef struct DRVCTRL_1{
  //const static uint8_t address = 0b00;
  union {
    uint32_t sr;
    struct {
      uint8_t cb : 8;
      bool phb : 1;
      uint8_t ca : 8;
      bool pha : 1;
    }bf_DRV_1 __attribute__((packed));
  };
}DRVCTRL_1_t;

typedef struct DRVCTRL_0 {
  //const static uint8_t address = 0b00;
  union {
    uint32_t sr;
    struct {
      uint8_t mres : 4;
      uint8_t : 4;
      bool dedge : 1;
      bool intpol : 1;
    };
  };
}DRVCTRL_0_t;

// namespace TMC2660_n {
//   struct CHOPCONF_t {
//     const static uint8_t address = 0b100;
//     union {
//       uint32_t sr;
//       struct {
//         uint8_t toff : 4;
//         uint8_t hstrt : 3;
//         uint8_t hend : 4;
//         uint8_t hdec : 2;
//         bool rndtf : 1;
//         bool chm : 1;
//         uint8_t tbl : 2;
//       };
//     };
//   };
// }

// struct SMARTEN_t {
//   const static uint8_t address = 0b101;
//   union {
//     uint32_t sr;
//     struct {
//       uint8_t semin : 4,
//                     : 1,
//               seup  : 2,
//                     : 1,
//               semax : 4,
//                     : 1,
//               sedn : 2;
//       bool seimin : 1;
//     };
//   };
// };

// struct SGCSCONF_t {
//   const static uint8_t address = 0b110;
//   union {
//     uint32_t sr;
//     struct {
//       uint8_t cs : 5;
//       uint8_t    : 3;
//       uint8_t sgt : 7;
//       uint8_t : 1;
//       bool sfilt : 1;
//     };
//   };
// };

// struct DRVCONF_t {
//   const static uint8_t address = 0b111;
//   union {
//     uint32_t sr;
//     struct {
//       uint8_t : 4;
//       uint8_t rdsel : 2;
//       bool vsense : 1;
//       bool sdoff : 1;
//       uint8_t ts2g : 2;
//       bool diss2g : 1;
//       uint8_t : 1;
//       uint8_t slpl : 2;
//       uint8_t slph : 2;
//       bool tst : 1;
//     };
//   };
// };

// struct READ_RDSEL00_t {
//   union {
//     uint32_t sr;
//     struct {
//       bool  sg_value : 1,
//             ot : 1,
//             otpw : 1,
//             s2ga : 1,
//             s2gb : 1,
//             ola : 1,
//             olb : 1,
//             stst : 1;
//       uint8_t : 2;
//       uint16_t mstep : 10;
//     };
//   };
// };

// struct READ_RDSEL01_t {
//   union {
//     uint32_t sr;
//     struct {
//       bool  sg_value : 1,
//             ot : 1,
//             otpw : 1,
//             s2ga : 1,
//             s2gb : 1,
//             ola : 1,
//             olb : 1,
//             stst : 1;
//       uint8_t : 2;
//       uint16_t sg_result : 10;
//     };
//   };
// };

// struct READ_RDSEL10_t {
//   union {
//     uint32_t sr;
//     struct {
//       bool  sg_value : 1,
//             ot : 1,
//             otpw : 1,
//             s2ga : 1,
//             s2gb : 1,
//             ola : 1,
//             olb : 1,
//             stst : 1;
//       uint8_t : 2;
//       uint8_t se : 5;
//       uint8_t sg_result : 5;
//     };
//   };
// };

#endif /* TMC_2660_REG_BITFIELDS_H_ */