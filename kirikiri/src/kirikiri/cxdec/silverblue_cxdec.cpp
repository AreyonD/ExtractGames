#include "cxdec.h"

static int xcode_building_stage0(struct cxdec_xcode_status *xcode, int stage);
static int xcode_building_stage1(struct cxdec_xcode_status *xcode, int stage);

static BYTE EncryptionControlBlock[4096] = {
        0x20, 0x45, 0x6e, 0x63, 0x72, 0x79, 0x70, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x63, 0x6f, 0x6e, 0x74, 
        0x72, 0x6f, 0x6c, 0x20, 0x62, 0x6c, 0x6f, 0x63, 0x6b, 0x20, 0x2d, 0x2d, 0x20, 0x53, 0x74, 0x61, 
        0x74, 0x69, 0x63, 0x61, 0x6c, 0x6c, 0x79, 0x20, 0x6f, 0x72, 0x20, 0x64, 0x79, 0x6e, 0x61, 0x6d, 
        0x69, 0x63, 0x61, 0x6c, 0x6c, 0x79, 0x2c, 0x20, 0x64, 0x69, 0x72, 0x65, 0x63, 0x74, 0x6c, 0x79, 
        0x20, 0x6f, 0x72, 0x20, 0x69, 0x6e, 0x64, 0x69, 0x72, 0x65, 0x63, 0x74, 0x6c, 0x79, 0x2c, 0x20, 
        0x75, 0x73, 0x69, 0x6e, 0x67, 0x20, 0x74, 0x68, 0x69, 0x73, 0x20, 0x70, 0x72, 0x6f, 0x67, 0x72, 
        0x61, 0x6d, 0x20, 0x61, 0x6e, 0x64, 0x2f, 0x6f, 0x72, 0x20, 0x62, 0x6c, 0x6f, 0x63, 0x6b, 0x20, 
        0x66, 0x72, 0x6f, 0x6d, 0x20, 0x6f, 0x74, 0x68, 0x65, 0x72, 0x20, 0x70, 0x72, 0x6f, 0x67, 0x72, 
        0x61, 0x6d, 0x73, 0x20, 0x77, 0x69, 0x6c, 0x6c, 0x20, 0x62, 0x65, 0x20, 0x69, 0x6c, 0x6c, 0x65, 
        0x67, 0x61, 0x6c, 0x20, 0x62, 0x79, 0x20, 0x74, 0x68, 0x65, 0x20, 0x6c, 0x69, 0x63, 0x65, 0x6e, 
        0x73, 0x65, 0x20, 0x61, 0x67, 0x72, 0x65, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x2e, 0x20, 0x82, 0xb1, 
        0x82, 0xcc, 0x83, 0x76, 0x83, 0x8d, 0x83, 0x4f, 0x83, 0x89, 0x83, 0x80, 0x82, 0xe2, 0x83, 0x75, 
        0x83, 0x8d, 0x83, 0x62, 0x83, 0x4e, 0x82, 0xf0, 0x81, 0x41, 0x90, 0xc3, 0x93, 0x49, 0x82, 0xc5, 
        0x82, 0xa0, 0x82, 0xea, 0x93, 0xae, 0x93, 0x49, 0x82, 0xc5, 0x82, 0xa0, 0x82, 0xea, 0x81, 0x41, 
        0x92, 0xbc, 0x90, 0xda, 0x93, 0x49, 0x82, 0xc5, 0x82, 0xa0, 0x82, 0xea, 0x8a, 0xd4, 0x90, 0xda, 
        0x93, 0x49, 0x82, 0xc5, 0x82, 0xa0, 0x82, 0xea, 0x81, 0x41, 0x91, 0xbc, 0x82, 0xcc, 0x83, 0x76, 
        0x83, 0x8d, 0x83, 0x4f, 0x83, 0x89, 0x83, 0x80, 0x82, 0xa9, 0x82, 0xe7, 0x97, 0x70, 0x82, 0xa2, 
        0x82, 0xe9, 0x82, 0xb1, 0x82, 0xc6, 0x82, 0xcd, 0x83, 0x89, 0x83, 0x43, 0x83, 0x5a, 0x83, 0x93, 
        0x83, 0x58, 0x82, 0xc9, 0x82, 0xe6, 0x82, 0xe8, 0x8b, 0xd6, 0x82, 0xb6, 0x82, 0xe7, 0x82, 0xea, 
        0x82, 0xc4, 0x82, 0xa2, 0x82, 0xdc, 0x82, 0xb7, 0x81, 0x42, 0x0a, 0x8e, 0x84, 0x8b, 0xa4, 0x82, 
        0xcc, 0x8d, 0xec, 0x95, 0x69, 0x82, 0xc9, 0x8b, 0xbb, 0x96, 0xa1, 0x82, 0xf0, 0x8e, 0x9d, 0x82, 
        0xc1, 0x82, 0xc4, 0x89, 0xba, 0x82, 0xb3, 0x82, 0xe8, 0x81, 0x41, 0x82, 0xa0, 0x82, 0xe8, 0x82, 
        0xaa, 0x82, 0xc6, 0x82, 0xa4, 0x82, 0xb2, 0x82, 0xb4, 0x82, 0xa2, 0x82, 0xdc, 0x82, 0xb7, 0x81, 
        0x49, 0x0a, 0x0a, 0x0a, 0x83, 0x41, 0x83, 0x8d, 0x83, 0x7d, 0x83, 0x8a, 0x83, 0x47, 0x82, 0xc5, 
        0x82, 0xcd, 0x81, 0x41, 0x91, 0xe5, 0x90, 0x6c, 0x82, 0xcc, 0x8f, 0x97, 0x90, 0xab, 0x82, 0xcc, 
        0x88, 0xd7, 0x82, 0xcc, 0x95, 0xa8, 0x8c, 0xea, 0x82, 0xf0, 0x8a, 0x46, 0x82, 0xb3, 0x82, 0xdc, 
        0x82, 0xc9, 0x82, 0xa8, 0x93, 0xcd, 0x82, 0xaf, 0x82, 0xb5, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xdc, 
        0x82, 0xb7, 0x81, 0x42, 0x0a, 0x89, 0x66, 0x89, 0xe6, 0x82, 0xe2, 0x8f, 0xac, 0x90, 0xe0, 0x82, 
        0xc6, 0x82, 0xcc, 0x88, 0xe1, 0x82, 0xa2, 0x82, 0xcd, 0x81, 0x41, 0x95, 0xa8, 0x8c, 0xea, 0x82, 
        0xcc, 0x8d, 0xc5, 0x8c, 0xe3, 0x82, 0xaa, 0x88, 0xea, 0x82, 0xc2, 0x82, 0xc5, 0x82, 0xcd, 0x82, 
        0xc8, 0x82, 0xa2, 0x82, 0xc6, 0x82, 0xb1, 0x82, 0xeb, 0x82, 0xc5, 0x82, 0xb7, 0x81, 0x42, 0x0a, 
        0x0a, 0x95, 0xa8, 0x8c, 0xea, 0x82, 0xf0, 0x93, 0xc7, 0x82, 0xdd, 0x90, 0x69, 0x82, 0xdf, 0x82, 
        0xe9, 0x82, 0xc6, 0x81, 0x41, 0x93, 0x72, 0x92, 0x86, 0x82, 0xc5, 0x81, 0x75, 0x95, 0xaa, 0x8a, 
        0xf2, 0x81, 0x69, 0x95, 0xaa, 0x82, 0xa9, 0x82, 0xea, 0x93, 0xb9, 0x81, 0x6a, 0x81, 0x76, 0x82, 
        0xaa, 0x94, 0xad, 0x90, 0xb6, 0x82, 0xb5, 0x82, 0xdc, 0x82, 0xb7, 0x81, 0x42, 0x0a, 0x97, 0xe1, 
        0x82, 0xa6, 0x82, 0xce, 0x93, 0xb9, 0x82, 0xf0, 0x89, 0x45, 0x82, 0xc9, 0x8b, 0xc8, 0x82, 0xaa, 
        0x82, 0xe9, 0x82, 0xa9, 0x81, 0x41, 0x8d, 0xb6, 0x82, 0xc9, 0x8b, 0xc8, 0x82, 0xaa, 0x82, 0xe9, 
        0x82, 0xa9, 0x81, 0x63, 0x0a, 0x82, 0xbb, 0x82, 0xa4, 0x82, 0xb5, 0x82, 0xc4, 0x91, 0x49, 0x82, 
        0xf1, 0x82, 0xbe, 0x90, 0xe6, 0x82, 0xc9, 0x82, 0xcd, 0x91, 0x53, 0x82, 0xad, 0x88, 0xe1, 0x82, 
        0xc1, 0x82, 0xbd, 0x83, 0x58, 0x83, 0x67, 0x81, 0x5b, 0x83, 0x8a, 0x81, 0x5b, 0x82, 0xaa, 0x93, 
        0x57, 0x8a, 0x4a, 0x82, 0xb3, 0x82, 0xea, 0x82, 0xe9, 0x81, 0x41, 0x20, 0x82, 0xbb, 0x82, 0xf1, 
        0x82, 0xc8, 0x83, 0x41, 0x83, 0x68, 0x83, 0x78, 0x83, 0x93, 0x83, 0x60, 0x83, 0x83, 0x81, 0x5b, 
        0x83, 0x51, 0x81, 0x5b, 0x83, 0x80, 0x8c, 0x60, 0x8e, 0xae, 0x82, 0xcc, 0x8d, 0xec, 0x95, 0x69, 
        0x82, 0xc5, 0x82, 0xb7, 0x81, 0x42, 0x0a, 0x88, 0xea, 0x93, 0x78, 0x83, 0x47, 0x83, 0x93, 0x83, 
        0x66, 0x83, 0x42, 0x83, 0x93, 0x83, 0x4f, 0x82, 0xf0, 0x8c, 0x7d, 0x82, 0xa6, 0x82, 0xc4, 0x82, 
        0xe0, 0x81, 0x41, 0x82, 0xe0, 0x82, 0xa4, 0x88, 0xea, 0x93, 0x78, 0x8d, 0xc5, 0x8f, 0x89, 0x82, 
        0xa9, 0x82, 0xe7, 0x82, 0xe2, 0x82, 0xe8, 0x92, 0xbc, 0x82, 0xb5, 0x8e, 0x9f, 0x82, 0xcd, 0x91, 
        0x4f, 0x89, 0xf1, 0x82, 0xc6, 0x88, 0xe1, 0x82, 0xa4, 0x91, 0x49, 0x91, 0xf0, 0x8e, 0x88, 0x82, 
        0xf0, 0x91, 0x49, 0x82, 0xd4, 0x82, 0xc6, 0x81, 0x41, 0x20, 0x82, 0xdc, 0x82, 0xbd, 0x88, 0xe1, 
        0x82, 0xa4, 0x95, 0xa8, 0x8c, 0xea, 0x82, 0xaa, 0x93, 0x57, 0x8a, 0x4a, 0x82, 0xb3, 0x82, 0xea, 
        0x82, 0xe9, 0x82, 0xcc, 0x82, 0xc5, 0x91, 0x53, 0x82, 0xc4, 0x82, 0xcc, 0x83, 0x47, 0x83, 0x93, 
        0x83, 0x66, 0x83, 0x42, 0x83, 0x93, 0x83, 0x4f, 0x82, 0xf0, 0x8c, 0xa9, 0x8f, 0x49, 0x82, 0xed, 
        0x82, 0xe9, 0x82, 0xdc, 0x82, 0xc5, 0x81, 0x41, 0x89, 0xbd, 0x93, 0x78, 0x82, 0xe0, 0x8a, 0x79, 
        0x82, 0xb5, 0x82, 0xde, 0x82, 0xb1, 0x82, 0xc6, 0x82, 0xaa, 0x8f, 0x6f, 0x97, 0x88, 0x82, 0xdc, 
        0x82, 0xb7, 0x81, 0x42, 0x20, 0x0a, 0x0a, 0x81, 0x77, 0x8a, 0xc3, 0x82, 0xa2, 0x91, 0xe4, 0x8e, 
        0x8c, 0x82, 0xf0, 0x9a, 0x91, 0x82, 0xa9, 0x82, 0xea, 0x81, 0x41, 0x83, 0x4c, 0x83, 0x58, 0x82, 
        0xc5, 0x8f, 0x49, 0x82, 0xe8, 0x81, 0x78, 0x82, 0xc5, 0x82, 0xcd, 0x82, 0xa0, 0x82, 0xe8, 0x82, 
        0xdc, 0x82, 0xb9, 0x82, 0xf1, 0x81, 0x41, 0x20, 0x82, 0xbb, 0x82, 0xcc, 0x90, 0xe6, 0x82, 0xcc, 
        0x8d, 0x73, 0x88, 0xd7, 0x82, 0xe2, 0x88, 0xa4, 0x8f, 0xee, 0x95, 0x5c, 0x8c, 0xbb, 0x82, 0xe0, 
        0x91, 0x8a, 0x8e, 0xe8, 0x82, 0xcc, 0x92, 0x6a, 0x90, 0xab, 0x83, 0x4c, 0x83, 0x83, 0x83, 0x89, 
        0x83, 0x4e, 0x83, 0x5e, 0x81, 0x5b, 0x82, 0xc9, 0x82, 0xe6, 0x82, 0xc1, 0x82, 0xc4, 0x97, 0x6c, 
        0x81, 0x58, 0x82, 0xc8, 0x95, 0xa8, 0x8c, 0xea, 0x82, 0xaa, 0x97, 0x70, 0x88, 0xd3, 0x82, 0xb3, 
        0x82, 0xea, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xdc, 0x82, 0xb7, 0x81, 0x42, 0x0a, 0x8d, 0xa1, 0x8d, 
        0xec, 0x82, 0xc5, 0x82, 0xcd, 0x81, 0x75, 0x95, 0x82, 0x8b, 0x43, 0x81, 0x76, 0x82, 0xc6, 0x82, 
        0xa2, 0x82, 0xa4, 0x83, 0x4c, 0x81, 0x5b, 0x83, 0x8f, 0x81, 0x5b, 0x83, 0x68, 0x82, 0xf0, 0x96, 
        0xda, 0x82, 0xc9, 0x82, 0xb7, 0x82, 0xe9, 0x82, 0xa9, 0x82, 0xe0, 0x82, 0xb5, 0x82, 0xea, 0x82, 
        0xdc, 0x82, 0xb9, 0x82, 0xf1, 0x82, 0xaa, 0x20, 0x82, 0xbb, 0x82, 0xea, 0x82, 0xcd, 0x82, 0xa0, 
        0x82, 0xad, 0x82, 0xdc, 0x82, 0xc5, 0x91, 0xe5, 0x90, 0x6c, 0x82, 0xcc, 0x95, 0xa8, 0x8c, 0xea, 
        0x82, 0xcc, 0x88, 0xd7, 0x82, 0xcc, 0x83, 0x47, 0x83, 0x62, 0x83, 0x5a, 0x83, 0x93, 0x83, 0x58, 
        0x82, 0xcc, 0x88, 0xea, 0x82, 0xc2, 0x82, 0xc5, 0x82, 0xb7, 0x81, 0x42, 0x0a, 0x91, 0x53, 0x94, 
        0x4e, 0x97, 0xee, 0x91, 0xce, 0x8f, 0xdb, 0x8d, 0xec, 0x95, 0x69, 0x82, 0xc5, 0x82, 0xcd, 0x96, 
        0xa1, 0x82, 0xed, 0x82, 0xa6, 0x82, 0xc8, 0x82, 0xa9, 0x82, 0xc1, 0x82, 0xbd, 0x81, 0x41, 0x82, 
        0xa2, 0x82, 0xdc, 0x82, 0xdc, 0x82, 0xc5, 0x82, 0xc9, 0x82, 0xc8, 0x82, 0xa2, 0x82, 0xe6, 0x82, 
        0xa4, 0x82, 0xc8, 0x90, 0x6c, 0x8a, 0xd4, 0x8a, 0xd6, 0x8c, 0x57, 0x82, 0xf0, 0x95, 0x60, 0x82, 
        0xab, 0x82, 0xbd, 0x82, 0xad, 0x82, 0xc4, 0x20, 0x8d, 0xa1, 0x89, 0xf1, 0x8e, 0x76, 0x82, 0xa2, 
        0x90, 0xd8, 0x82, 0xc1, 0x82, 0xc4, 0x8e, 0xe6, 0x82, 0xe8, 0x93, 0xfc, 0x82, 0xea, 0x82, 0xdc, 
        0x82, 0xb5, 0x82, 0xbd, 0x81, 0x42, 0x0a, 0x0a, 0x8c, 0xbb, 0x8e, 0xc0, 0x82, 0xc5, 0x82, 0xcd, 
        0x91, 0xcc, 0x8c, 0xb1, 0x82, 0xc5, 0x82, 0xab, 0x82, 0xc8, 0x82, 0xa2, 0x82, 0xe6, 0x82, 0xa4, 
        0x82, 0xc8, 0x95, 0xa8, 0x8c, 0xea, 0x82, 0xf0, 0x82, 0xb2, 0x97, 0x70, 0x88, 0xd3, 0x82, 0xb5, 
        0x82, 0xdc, 0x82, 0xb5, 0x82, 0xbd, 0x81, 0x42, 0x0a, 0x83, 0x51, 0x81, 0x5b, 0x83, 0x80, 0x8d, 
        0xec, 0x95, 0x69, 0x82, 0xc8, 0x82, 0xe7, 0x82, 0xc5, 0x82, 0xcd, 0x82, 0xcc, 0x94, 0xf1, 0x93, 
        0xfa, 0x8f, 0xed, 0x82, 0xf0, 0x82, 0xa8, 0x8a, 0x79, 0x82, 0xb5, 0x82, 0xdd, 0x82, 0xa2, 0x82, 
        0xbd, 0x82, 0xbe, 0x82, 0xaf, 0x82, 0xea, 0x82, 0xce, 0x82, 0xc6, 0x8e, 0x76, 0x82, 0xa2, 0x82, 
        0xdc, 0x82, 0xb7, 0x81, 0x42, 0x0a, 0x0a, 0x91, 0x4f, 0x8d, 0xec, 0x82, 0xa9, 0x82, 0xe7, 0x89, 
        0x9e, 0x89, 0x87, 0x82, 0xad, 0x82, 0xbe, 0x82, 0xb3, 0x82, 0xc1, 0x82, 0xc4, 0x82, 0xa2, 0x82, 
        0xe9, 0x83, 0x86, 0x81, 0x5b, 0x83, 0x55, 0x81, 0x5b, 0x97, 0x6c, 0x82, 0xe0, 0x82, 0xa2, 0x82, 
        0xc2, 0x82, 0xe0, 0x96, 0x7b, 0x93, 0x96, 0x82, 0xc9, 0x82, 0xa0, 0x82, 0xe8, 0x82, 0xaa, 0x82, 
        0xc6, 0x82, 0xa4, 0x82, 0xb2, 0x82, 0xb4, 0x82, 0xa2, 0x82, 0xdc, 0x82, 0xb7, 0x81, 0x49, 0x0a, 
        0x8d, 0x95, 0x82, 0xb3, 0x81, 0x41, 0x8e, 0xb9, 0x93, 0x69, 0x81, 0x41, 0x93, 0xc6, 0x90, 0xe8, 
        0x97, 0x7e, 0x81, 0x41, 0x8e, 0x4f, 0x8a, 0x70, 0x8a, 0xd6, 0x8c, 0x57, 0x82, 0xe0, 0x91, 0xe5, 
        0x96, 0xda, 0x82, 0xc9, 0x90, 0xb7, 0x82, 0xe8, 0x8d, 0x9e, 0x82, 0xdd, 0x82, 0xdc, 0x82, 0xb7, 
        0x81, 0x42, 0x0a, 0x82, 0xc7, 0x82, 0xa4, 0x82, 0xbc, 0x82, 0xb2, 0x8a, 0xfa, 0x91, 0xd2, 0x89, 
        0xba, 0x82, 0xb3, 0x82, 0xa2, 0x81, 0x49, 0x0a, 0x0a, 0x0a, 0x83, 0x41, 0x20, 0x83, 0x8d, 0x20, 
        0x83, 0x7d, 0x20, 0x83, 0x8a, 0x20, 0x83, 0x47, 0x0a, 0xf4, 0xd7, 0x8f, 0xf5, 0x0c, 0xb1, 0xa3, 
        0x51, 0x93, 0xb0, 0x1b, 0x6f, 0x52, 0xf5, 0x05, 0xcf, 0xa5, 0xdd, 0x02, 0x8c, 0xff, 0x40, 0x5f, 
        0x12, 0x9f, 0x0d, 0x5c, 0xe7, 0x6a, 0xbb, 0xa1, 0x6a, 0x32, 0x4a, 0x20, 0x1d, 0xa2, 0x23, 0xb4, 
        0x42, 0xf7, 0x6b, 0x33, 0x43, 0xea, 0x84, 0x3f, 0xb6, 0xb9, 0x4d, 0xd8, 0xf4, 0x6f, 0x3e, 0x60, 
        0xb1, 0xfb, 0xd2, 0x8a, 0x5e, 0xba, 0xb0, 0x60, 0x74, 0x4b, 0x3f, 0x50, 0x4f, 0x9d, 0x93, 0x0b, 
        0x43, 0xa7, 0xe0, 0x39, 0xf1, 0x50, 0x2b, 0x38, 0x88, 0xb6, 0xd8, 0xb9, 0x75, 0xab, 0x27, 0xab, 
        0x2c, 0x27, 0x1a, 0x5a, 0x62, 0xca, 0x05, 0x83, 0x77, 0x76, 0x3a, 0x09, 0xd3, 0x03, 0x20, 0xc3, 
        0x3b, 0x31, 0x29, 0x96, 0x2d, 0xf3, 0xde, 0x8c, 0x3e, 0x95, 0x67, 0x10, 0x4f, 0xc9, 0x46, 0xb0, 
        0xe9, 0x30, 0xe9, 0xf7, 0xe4, 0xdc, 0x9b, 0xd8, 0x98, 0xec, 0x4f, 0x05, 0x46, 0x2b, 0x41, 0xbe, 
        0xac, 0x67, 0xff, 0xb6, 0xd0, 0x96, 0x6a, 0x9d, 0xa5, 0xcf, 0x7e, 0x1a, 0x9b, 0x64, 0xfa, 0xa2, 
        0x33, 0xca, 0x64, 0xdd, 0x27, 0x84, 0x15, 0xd5, 0xfa, 0xe6, 0x2c, 0x05, 0x5a, 0xc4, 0x6e, 0x6e, 
        0xfa, 0x6d, 0xa6, 0xd9, 0x5b, 0x3d, 0x6b, 0x5f, 0xce, 0xf2, 0x30, 0x67, 0xb4, 0xaa, 0xe7, 0xb3, 
        0xd6, 0x2f, 0x36, 0xa3, 0xb4, 0x99, 0x1b, 0xff, 0xda, 0x47, 0x8d, 0x1a, 0x13, 0x3e, 0xb3, 0xec, 
        0xda, 0x0c, 0x9d, 0xa5, 0x90, 0x43, 0x55, 0x9d, 0x8c, 0xf1, 0x27, 0xa0, 0x38, 0xe1, 0xb3, 0x81, 
        0x24, 0x15, 0xca, 0x1c, 0xd5, 0x78, 0x79, 0xf2, 0xf5, 0x73, 0x48, 0xf0, 0x0a, 0xdd, 0x45, 0x16, 
        0x9b, 0x15, 0x4e, 0x91, 0x24, 0x56, 0x1d, 0xa2, 0xd8, 0xd2, 0x4f, 0x2c, 0x1b, 0x4b, 0xab, 0x2a, 
        0x03, 0x61, 0x24, 0x34, 0x7c, 0x8e, 0x60, 0x1d, 0x0e, 0x51, 0xf2, 0x6f, 0xae, 0xbb, 0x57, 0xf9, 
        0x0b, 0x54, 0xf7, 0x9c, 0xdf, 0xb5, 0x7e, 0x10, 0xff, 0xd2, 0x18, 0x0a, 0x63, 0xc7, 0xad, 0x0d, 
        0x5c, 0x74, 0x59, 0x2a, 0xed, 0xb2, 0xc6, 0xf4, 0xcb, 0x6f, 0xa5, 0x18, 0xae, 0x35, 0x62, 0xca, 
        0x7c, 0xc2, 0xc4, 0xc6, 0xf2, 0x3c, 0xc5, 0x6c, 0x52, 0xab, 0x98, 0x8b, 0x6c, 0xc3, 0x1c, 0xb3, 
        0x1d, 0xb4, 0x14, 0x8f, 0xee, 0x7c, 0x20, 0x3c, 0x38, 0x2d, 0x51, 0xbf, 0x7f, 0x7b, 0x74, 0x64, 
        0x58, 0xd9, 0xba, 0xe2, 0x0e, 0xbf, 0x56, 0x90, 0x34, 0xc2, 0x47, 0x06, 0xa4, 0x00, 0x32, 0x35, 
        0x96, 0x2e, 0xc1, 0x0d, 0xde, 0x45, 0x3d, 0xd3, 0xe3, 0xb0, 0x38, 0x0a, 0x6c, 0x61, 0x2e, 0x9f, 
        0xd8, 0x93, 0xc2, 0xdb, 0x39, 0xaf, 0xa6, 0xd8, 0x90, 0xcd, 0x63, 0x22, 0x4f, 0x18, 0x76, 0x73, 
        0xfe, 0xf3, 0x22, 0x8b, 0x6a, 0x4f, 0xbb, 0x56, 0xdd, 0x1f, 0x71, 0x38, 0xcf, 0x13, 0xc5, 0xdf, 
        0x4e, 0x95, 0x07, 0xdb, 0x9c, 0x0d, 0x41, 0xf6, 0xd0, 0xc8, 0x2c, 0x9f, 0xf4, 0x63, 0xcb, 0x0d, 
        0x0e, 0x13, 0xde, 0x09, 0x59, 0x9f, 0x11, 0xa9, 0x4f, 0xb7, 0x67, 0xc5, 0x5b, 0xfc, 0x03, 0x85, 
        0x5c, 0xd2, 0x39, 0xa5, 0xa8, 0xe0, 0x43, 0xb0, 0x44, 0xfd, 0x68, 0x7e, 0xdd, 0x11, 0xe1, 0xb9, 
        0x84, 0x47, 0xde, 0xdb, 0x72, 0xfa, 0xb5, 0xc3, 0xb1, 0x18, 0xd8, 0xc6, 0xd5, 0x8d, 0x45, 0x99, 
        0x3b, 0x05, 0x5b, 0x4c, 0x0c, 0xbf, 0xdf, 0x8d, 0xa4, 0x14, 0x1d, 0xd4, 0x79, 0x75, 0xc3, 0xd2, 
        0x32, 0x1b, 0x10, 0x25, 0xcf, 0x8e, 0x34, 0x3f, 0x13, 0x77, 0x58, 0x23, 0xd6, 0xca, 0xad, 0xcb, 
        0x25, 0x6a, 0x46, 0x39, 0xcf, 0xab, 0x8d, 0x5c, 0xa6, 0xef, 0x3a, 0x5a, 0xdd, 0x37, 0x79, 0xc7, 
        0x3c, 0x5e, 0x36, 0x25, 0xb2, 0x1a, 0xbe, 0x4c, 0x08, 0x8d, 0x4d, 0x48, 0xbb, 0x1e, 0xdf, 0x78, 
        0xda, 0xd3, 0xa4, 0x6a, 0x3c, 0x65, 0xc5, 0xfd, 0x86, 0x4b, 0xce, 0x61, 0x5f, 0xf1, 0x55, 0x49, 
        0xdd, 0x33, 0xe5, 0xd7, 0x1f, 0xe3, 0x08, 0x5e, 0x26, 0x25, 0x24, 0x74, 0x25, 0x36, 0x24, 0xc5, 
        0xae, 0xf5, 0xf6, 0x9f, 0xe0, 0xf1, 0xa6, 0x75, 0x17, 0x35, 0xf1, 0x77, 0xca, 0x00, 0xb8, 0x4f, 
        0xa0, 0x92, 0x07, 0x4f, 0xde, 0xf0, 0x8a, 0xac, 0x66, 0xa1, 0x07, 0x4a, 0xd7, 0xff, 0x5b, 0xf9, 
        0xc7, 0x1b, 0x90, 0x2b, 0xec, 0xbc, 0xe4, 0xde, 0xad, 0x4f, 0x78, 0xf5, 0x7b, 0x2a, 0x9b, 0xe1, 
        0x5b, 0x35, 0x5a, 0x73, 0x8a, 0x9d, 0xd0, 0x80, 0x28, 0xe5, 0xbe, 0xc3, 0x3d, 0x52, 0xe4, 0xf2, 
        0xe2, 0x91, 0x19, 0x39, 0xb8, 0xf4, 0x32, 0x96, 0x1e, 0x51, 0x52, 0x71, 0xb9, 0xc5, 0x07, 0x61, 
        0x9f, 0x2e, 0xa1, 0x2e, 0xd4, 0xe9, 0x7a, 0xca, 0xd0, 0x68, 0x1c, 0xdb, 0x03, 0xc8, 0x75, 0x9f, 
        0x0f, 0x69, 0x0d, 0xc4, 0x5e, 0x87, 0x9e, 0xbc, 0x28, 0x75, 0xff, 0xea, 0x48, 0xab, 0x9e, 0x2d, 
        0xa8, 0x5e, 0x60, 0x3d, 0xc0, 0xa9, 0x3d, 0x3e, 0x42, 0x38, 0x8d, 0x10, 0x9a, 0x3a, 0xc7, 0x45, 
        0xf0, 0x07, 0x6d, 0x5e, 0x3a, 0xce, 0x33, 0x86, 0xec, 0x88, 0x15, 0x43, 0xc4, 0x4b, 0x92, 0xfe, 
        0x04, 0xbd, 0x80, 0xf6, 0x97, 0x75, 0xe1, 0x11, 0xf9, 0x64, 0x90, 0xe7, 0xc7, 0x6c, 0xa6, 0xab, 
        0xae, 0x76, 0xfb, 0xda, 0x01, 0xe3, 0x95, 0x7e, 0x0f, 0x6f, 0x3a, 0xd2, 0x9d, 0xf8, 0xa9, 0x9e, 
        0xc2, 0x58, 0x42, 0x6d, 0xe3, 0xea, 0x73, 0x60, 0x76, 0x11, 0x38, 0x36, 0x14, 0xe4, 0x85, 0xee, 
        0x73, 0xc4, 0x90, 0x23, 0x76, 0x22, 0x8a, 0xb0, 0xb8, 0x0e, 0xae, 0x6e, 0xc3, 0x4b, 0xf2, 0x69, 
        0x8a, 0xbc, 0xa5, 0x4e, 0x54, 0xb4, 0x02, 0xca, 0x38, 0x79, 0x5c, 0xba, 0xcc, 0xe7, 0xd9, 0x3f, 
        0x4f, 0xff, 0x97, 0x73, 0xb5, 0xce, 0xdc, 0x7e, 0xa5, 0xe0, 0xf0, 0x56, 0xc1, 0x1b, 0x78, 0xe2, 
        0x54, 0x8d, 0x40, 0x17, 0xf0, 0xeb, 0xc2, 0xb1, 0xa6, 0x56, 0x56, 0x03, 0xe0, 0x25, 0x5b, 0x90, 
        0xb6, 0x60, 0x11, 0x5c, 0x6b, 0x3d, 0x9a, 0xac, 0x65, 0xc3, 0x86, 0x1c, 0x6e, 0xaa, 0x21, 0xd2, 
        0x2e, 0x04, 0x6d, 0x19, 0xbd, 0x66, 0xfe, 0x9d, 0x0d, 0x79, 0xd5, 0x24, 0xe7, 0x5d, 0x59, 0x9b, 
        0xa0, 0xa0, 0x0f, 0x03, 0x9a, 0x4a, 0x65, 0x55, 0x1e, 0xac, 0x47, 0xd6, 0x3d, 0xfa, 0x23, 0x30, 
        0xab, 0x5b, 0xd8, 0xbf, 0xd9, 0x3f, 0xff, 0xa9, 0xbb, 0x97, 0x5c, 0x35, 0xba, 0x57, 0x33, 0x2b, 
        0x39, 0xfa, 0xbc, 0x50, 0x8a, 0x11, 0x74, 0x14, 0xc8, 0x0e, 0xe1, 0x6c, 0x1f, 0x51, 0x8b, 0x19, 
        0xde, 0xe2, 0x15, 0x30, 0xb2, 0x78, 0x67, 0xb5, 0x80, 0xf9, 0x42, 0x20, 0xf4, 0xe7, 0x5e, 0x31, 
        0xfd, 0x52, 0x83, 0x40, 0x43, 0x04, 0x12, 0xdc, 0xef, 0x26, 0xd5, 0x2c, 0xda, 0x5c, 0x35, 0x54, 
        0x72, 0xfc, 0x79, 0xab, 0xce, 0xd4, 0xda, 0xb2, 0x45, 0x6a, 0xb0, 0xb7, 0x07, 0x7f, 0x65, 0xb0, 
        0x94, 0xdc, 0x52, 0x24, 0xa3, 0xba, 0x1c, 0x31, 0xa5, 0x31, 0x76, 0x3a, 0xda, 0x09, 0x57, 0x53, 
        0x64, 0xdf, 0xdd, 0x44, 0xb4, 0xdb, 0x90, 0xea, 0x44, 0xfc, 0xa7, 0x9c, 0xf9, 0x0d, 0xe4, 0x72, 
        0x62, 0x92, 0x6f, 0x51, 0x9d, 0x9d, 0xb6, 0x5d, 0xfc, 0x82, 0xef, 0xb9, 0x50, 0x89, 0xa9, 0xd2, 
        0x46, 0x9b, 0x70, 0xe1, 0xbf, 0x78, 0x66, 0x25, 0x4b, 0x41, 0x79, 0xf8, 0x22, 0x96, 0xbe, 0x82, 
        0x64, 0x71, 0x70, 0x9e, 0x77, 0x5a, 0x04, 0xed, 0xa0, 0xce, 0x3b, 0xd5, 0xac, 0x75, 0xf2, 0x2e, 
        0xfd, 0xfa, 0xb3, 0x7d, 0x70, 0xad, 0x3c, 0x05, 0x2d, 0x64, 0xcc, 0x76, 0x20, 0x34, 0x56, 0xff, 
        0x7a, 0x0a, 0x42, 0xb8, 0x0a, 0x72, 0xd2, 0x01, 0x8a, 0xe1, 0x5e, 0x3a, 0x37, 0x77, 0x26, 0x5a, 
        0x03, 0xa3, 0xfc, 0x56, 0x0d, 0x7d, 0x35, 0x3c, 0xc0, 0x6f, 0x4d, 0x95, 0x52, 0x53, 0xb6, 0x60, 
        0x08, 0xd3, 0xa4, 0xb0, 0x85, 0x3e, 0xfe, 0xe0, 0xcc, 0x8a, 0x98, 0x69, 0xca, 0xc0, 0x37, 0x5c, 
        0xa5, 0x0b, 0x74, 0x3f, 0x1b, 0xaf, 0x8c, 0x90, 0xbe, 0x85, 0x69, 0xeb, 0xbf, 0xb4, 0xfd, 0x1a, 
        0x74, 0xd6, 0xaa, 0x6c, 0x5a, 0xd5, 0xe9, 0x93, 0xcb, 0xa9, 0xc6, 0xc8, 0x16, 0xc0, 0x5b, 0x8e, 
        0x46, 0xbf, 0x9a, 0xdf, 0x78, 0xdf, 0x8a, 0x9f, 0x47, 0xc8, 0xa3, 0x4c, 0x40, 0x5d, 0x49, 0x7c, 
        0x3a, 0x1f, 0x3c, 0xd3, 0x3e, 0xa7, 0x4b, 0xa4, 0x7f, 0x36, 0xd1, 0x13, 0x0f, 0x3f, 0xde, 0xcf, 
        0xca, 0xde, 0x3f, 0x60, 0x12, 0xcd, 0x92, 0x85, 0x7e, 0xf1, 0x11, 0xa7, 0xb0, 0x25, 0x92, 0x33, 
        0xda, 0x02, 0x47, 0xd2, 0x8c, 0xf4, 0xad, 0x88, 0x66, 0x2c, 0x3e, 0x85, 0x69, 0x27, 0x50, 0x4f, 
        0x9a, 0x8c, 0xba, 0x41, 0x2f, 0x0f, 0x96, 0x37, 0xb9, 0xce, 0xe0, 0x09, 0x83, 0xba, 0xd1, 0x47, 
        0x57, 0x20, 0x78, 0x2a, 0x42, 0xc3, 0x6f, 0x33, 0xfe, 0x17, 0x1d, 0x62, 0x80, 0xb9, 0x43, 0x57, 
        0x36, 0x30, 0x23, 0x95, 0xd7, 0xfa, 0x21, 0x3c, 0x9f, 0xae, 0x4b, 0x98, 0xfa, 0x8e, 0x91, 0xbd, 
        0x99, 0xc7, 0xe4, 0xff, 0x22, 0x6e, 0x71, 0x5c, 0x2c, 0x1d, 0x9b, 0x3c, 0xd3, 0xf7, 0x6e, 0x70, 
        0xed, 0x84, 0x2a, 0x32, 0x55, 0x6c, 0xfe, 0xdf, 0x84, 0x76, 0x2a, 0xb9, 0x73, 0x63, 0x07, 0x72, 
        0x6b, 0x42, 0xf4, 0xca, 0xb1, 0x23, 0x5c, 0xe3, 0x6b, 0xe1, 0x23, 0xc1, 0x27, 0xe8, 0xee, 0xb6, 
        0xa6, 0xe9, 0x0d, 0x03, 0x19, 0x53, 0x7b, 0xfe, 0xf8, 0xed, 0x10, 0x6b, 0x48, 0x24, 0x65, 0x57, 
        0x1d, 0xe2, 0xd2, 0x08, 0x4b, 0x53, 0xb6, 0x39, 0xf4, 0x9f, 0xf5, 0x5c, 0x01, 0xa5, 0xa2, 0x1f, 
        0x99, 0xc3, 0x27, 0xc7, 0x37, 0x05, 0x8f, 0x4f, 0x20, 0xf1, 0x99, 0x7b, 0x29, 0x8f, 0x26, 0xa1, 
        0x02, 0x9d, 0x40, 0x3a, 0x8d, 0x44, 0x32, 0x12, 0x5c, 0x26, 0x38, 0x0f, 0xbd, 0xb0, 0xeb, 0x1e, 
        0x15, 0xf7, 0x5c, 0x8f, 0x89, 0x99, 0xc3, 0x06, 0x09, 0x93, 0xb3, 0xbe, 0x9f, 0xa2, 0xd7, 0x46, 
        0x02, 0xef, 0x40, 0x34, 0x59, 0x60, 0x00, 0xb6, 0xf3, 0x16, 0x53, 0xfe, 0x7a, 0xcb, 0x03, 0x7f, 
        0x6d, 0x12, 0xaf, 0x05, 0xa8, 0xe9, 0xa1, 0x30, 0xc1, 0xe5, 0xf1, 0x8b, 0x2f, 0x65, 0x99, 0x52, 
        0xd8, 0x50, 0x29, 0xdf, 0xb4, 0x66, 0x96, 0x14, 0xa3, 0x69, 0x33, 0xe7, 0xe9, 0xfa, 0xf4, 0x20, 
        0xf3, 0x73, 0xdf, 0x10, 0x86, 0x38, 0x8a, 0x3b, 0x5a, 0x21, 0x7b, 0x0e, 0x48, 0x1e, 0x21, 0xf6, 
        0x55, 0x66, 0xb8, 0x67, 0xc9, 0xd7, 0x30, 0x5f, 0xba, 0x56, 0x19, 0xc4, 0x24, 0x2c, 0x9e, 0x71, 
        0x93, 0x3f, 0x33, 0xb1, 0xd7, 0x0a, 0x32, 0x6c, 0x7a, 0x53, 0xab, 0xd6, 0xeb, 0x6c, 0x39, 0xf7, 
        0x1a, 0xa5, 0xee, 0x8d, 0x07, 0xba, 0x8a, 0x78, 0x87, 0xed, 0x99, 0x29, 0x44, 0x8f, 0xb3, 0x3b, 
        0x08, 0x1c, 0x92, 0x39, 0x13, 0x23, 0x23, 0x08, 0x1d, 0x9b, 0xef, 0x79, 0x8d, 0x8e, 0xbd, 0xdc, 
        0x61, 0xc6, 0x4d, 0xe6, 0x9f, 0x26, 0x88, 0x6a, 0x5f, 0x90, 0xcf, 0xf9, 0xe9, 0x13, 0xbb, 0x47, 
        0xa7, 0x44, 0x4e, 0x50, 0xd4, 0x33, 0xe8, 0xdd, 0x51, 0xaf, 0x5b, 0xca, 0x66, 0x9b, 0x51, 0xf3, 
        0xe5, 0x07, 0x2d, 0xd9, 0x17, 0x4d, 0xef, 0xe3, 0x7a, 0x36, 0x13, 0x21, 0xc6, 0x11, 0x42, 0x33, 
        0x13, 0x0a, 0x76, 0x30, 0x5c, 0xf7, 0x51, 0xb7, 0x28, 0xc6, 0x54, 0xca, 0xd4, 0x6b, 0x97, 0x99, 
        0x64, 0x38, 0xba, 0x66, 0x3a, 0x5d, 0x4b, 0x12, 0x27, 0x69, 0x29, 0x8c, 0xe2, 0xf5, 0x97, 0xb1, 
        0x87, 0x8d, 0xa0, 0x58, 0xb0, 0xb6, 0x43, 0x09, 0xb9, 0x5e, 0x48, 0xb5, 0x57, 0x2a, 0xda, 0x05, 
        0xb4, 0x10, 0xf4, 0x4a, 0x14, 0x9f, 0xa8, 0xb0, 0x72, 0xb8, 0x26, 0x7c, 0xd6, 0x2d, 0x1d, 0x38, 
        0x0f, 0x17, 0xe8, 0xdb, 0x16, 0xf5, 0xb5, 0xee, 0xfd, 0x44, 0xe3, 0x50, 0xb6, 0xda, 0xf8, 0x6e, 
        0xfa, 0xd8, 0xaf, 0x5d, 0x58, 0xeb, 0xeb, 0xf2, 0xc5, 0xc4, 0xe0, 0x97, 0x15, 0xeb, 0x4a, 0x51, 
        0x4d, 0x74, 0x28, 0x1f, 0xaa, 0x91, 0xb4, 0xfd, 0x06, 0x6f, 0x38, 0x42, 0x79, 0xc0, 0x81, 0xe0, 
        0x73, 0x5d, 0x92, 0x45, 0xdd, 0x6f, 0x47, 0xe7, 0x44, 0xc5, 0x95, 0xd7, 0x9e, 0x37, 0x2b, 0x5c, 
        0x69, 0xa3, 0x24, 0x0f, 0x1d, 0x8a, 0x61, 0x59, 0x23, 0x58, 0xaf, 0x08, 0xfd, 0xf4, 0x9a, 0x4a, 
        0xad, 0xb2, 0xca, 0xff, 0x3c, 0x2e, 0x49, 0x80, 0x2d, 0x74, 0xd8, 0x3a, 0xcb, 0xba, 0xdf, 0x39, 
        0x09, 0x36, 0x42, 0x68, 0xf4, 0x34, 0x70, 0x21, 0xfd, 0x79, 0x22, 0xa0, 0x32, 0x64, 0x8e, 0xf3, 
        0xce, 0x39, 0xc5, 0x9e, 0x53, 0xed, 0x4e, 0x83, 0x21, 0x20, 0xac, 0x41, 0x37, 0x76, 0x99, 0x9f, 
        0x6c, 0xa4, 0x6c, 0x00, 0x75, 0x27, 0xa8, 0xd0, 0x4c, 0xf5, 0x40, 0x5e, 0xde, 0xc0, 0x77, 0xa1, 
        0xcc, 0x7e, 0xd8, 0x73, 0x55, 0x46, 0x09, 0x37, 0x44, 0x2c, 0xe8, 0xc1, 0x3f, 0xac, 0x20, 0x28, 
        0x24, 0x7c, 0x4f, 0xeb, 0xad, 0xff, 0x66, 0x3d, 0x21, 0x6f, 0x1c, 0xfa, 0x5a, 0x0d, 0x4a, 0xf1, 
        0xb2, 0x8f, 0x2b, 0x6a, 0x87, 0xf7, 0xb0, 0x3b, 0xbf, 0x7e, 0xcc, 0x75, 0x97, 0x72, 0x01, 0x43, 
        0xd6, 0x4a, 0xad, 0xd5, 0xcb, 0x6a, 0x4d, 0x1c, 0x4e, 0x0f, 0x0c, 0xb2, 0x53, 0xa5, 0x7b, 0x13, 
        0x92, 0xfb, 0x59, 0xbe, 0x84, 0x12, 0xbd, 0xc0, 0x4f, 0x10, 0xd8, 0x6a, 0x06, 0xb6, 0xc8, 0xe1, 
        0xfa, 0xb2, 0x5a, 0x3b, 0x25, 0x2f, 0x7d, 0x9c, 0xb8, 0xa2, 0xec, 0xb2, 0x3b, 0xa4, 0x43, 0x7b, 
        0xfd, 0x7a, 0xd2, 0x7f, 0x78, 0xee, 0xbf, 0x3b, 0x03, 0xf1, 0x8f, 0xb1, 0x27, 0x24, 0x5c, 0xc9, 
        0x21, 0xed, 0x44, 0xf1, 0x52, 0x14, 0xf1, 0x47, 0xab, 0x49, 0x38, 0x0e, 0xc6, 0x7a, 0xef, 0xe2, 
        0x9b, 0xc4, 0xea, 0x54, 0x67, 0xe3, 0xd7, 0x34, 0x01, 0xb3, 0x4c, 0x62, 0xa9, 0xc0, 0xf0, 0x55, 
        0xdb, 0xb5, 0x95, 0x5e, 0x2d, 0x75, 0xf3, 0x37, 0x76, 0x68, 0x57, 0xc0, 0x4b, 0x3d, 0x61, 0x13, 
        0x68, 0x95, 0x39, 0x24, 0x18, 0x90, 0x3d, 0x8a, 0x71, 0x7c, 0xba, 0x15, 0x0f, 0xe1, 0x14, 0xa3, 
        0xbc, 0x59, 0xcf, 0xac, 0xe8, 0x11, 0x26, 0xcb, 0x69, 0xe7, 0xed, 0x01, 0x7c, 0x57, 0x0d, 0xfe, 
        0xf4, 0xd2, 0xa9, 0xeb, 0x92, 0xe7, 0xb9, 0x9d, 0x7f, 0xd6, 0xdb, 0xb6, 0x21, 0xa2, 0xa4, 0x96, 
        0x3e, 0x8e, 0xda, 0x1a, 0x76, 0xbf, 0x7b, 0xa7, 0x6e, 0x42, 0x56, 0xd4, 0x85, 0xf2, 0xfe, 0xa8, 
        0xd2, 0x2d, 0x8d, 0xfb, 0xb6, 0x7c, 0x2b, 0x23, 0xbb, 0xe2, 0x82, 0x99, 0xe9, 0xf1, 0xd5, 0x22, 
        0xf1, 0x1a, 0x5f, 0x3a, 0x0f, 0xd4, 0xbf, 0x00, 0xee, 0xda, 0x30, 0xfa, 0x2f, 0xc3, 0x81, 0xda, 
        0x19, 0xa1, 0x76, 0x29, 0xf3, 0x3c, 0x85, 0xe4, 0x9c, 0xc0, 0xda, 0x63, 0x15, 0x93, 0xdd, 0x94, 
        0x23, 0x49, 0x0b, 0x72, 0x0f, 0xbd, 0x3a, 0x33, 0x25, 0x24, 0xfd, 0x4b, 0x41, 0x0a, 0x66, 0x9f, 
        0xc5, 0xac, 0x7d, 0xcd, 0x91, 0xd3, 0x7e, 0x3d, 0x19, 0x58, 0xdf, 0xbe, 0x2b, 0x8c, 0xb8, 0xb7, 
        0x7d, 0xab, 0xe0, 0xbf, 0x76, 0x2b, 0xfe, 0x30, 0x79, 0x17, 0xe5, 0x42, 0xd7, 0xf3, 0x22, 0x16, 
        0xb5, 0xdd, 0x8e, 0x12, 0xea, 0xc3, 0xcf, 0x61, 0xad, 0xe2, 0x12, 0x27, 0xaf, 0x39, 0x44, 0x7f, 
        0xe6, 0x53, 0x34, 0x5e, 0x08, 0x0f, 0xfe, 0x28, 0x10, 0xc0, 0xe3, 0xf2, 0x34, 0x6c, 0x41, 0x84, 
        0x4f, 0xab, 0x68, 0x8b, 0xab, 0x62, 0xd6, 0x03, 0x6c, 0xbf, 0xb7, 0xb1, 0xbe, 0x48, 0x87, 0x83, 
        0xdc, 0xe7, 0xb1, 0x22, 0x4d, 0xc4, 0x55, 0x14, 0x53, 0x41, 0xf5, 0x2a, 0x5d, 0x22, 0x4d, 0x74, 
        0x77, 0xa1, 0x9f, 0x9c, 0x17, 0x4b, 0x87, 0x2e, 0xb7, 0x51, 0xf6, 0xaf, 0xf7, 0x7b, 0x06, 0x0c, 
        0xc2, 0x74, 0x54, 0xb2, 0xa2, 0xb4, 0x1d, 0x7d, 0x82, 0xbd, 0x5e, 0xd5, 0x99, 0xad, 0x2a, 0xed, 
        0x2b, 0xca, 0xcc, 0xae, 0xf5, 0x9b, 0xd6, 0xff, 0x89, 0x7c, 0x4f, 0x0d, 0x4b, 0x2d, 0x8d, 0x27, 
        0xfc, 0x19, 0x21, 0x38, 0x76, 0x50, 0x68, 0x94, 0x3e, 0xeb, 0x76, 0xae, 0x07, 0x66, 0x8b, 0x63, 
        0x39, 0xc7, 0xc8, 0x2d, 0x8d, 0x03, 0x10, 0x25, 0xdf, 0xcd, 0x7f, 0x08, 0x52, 0xb3, 0xad, 0x88, 
        0xf3, 0x79, 0x5a, 0x33, 0x0d, 0x19, 0x48, 0x9b, 0xc0, 0x1a, 0xa5, 0xf4, 0x58, 0x67, 0x7c, 0xca, 
        0x05, 0x50, 0x55, 0x58, 0x0d, 0x1a, 0x82, 0xa8, 0x37, 0xd2, 0xde, 0x31, 0xe9, 0x01, 0x76, 0x75, 
        0xa7, 0xfd, 0xac, 0xb6, 0xf1, 0x7b, 0xf7, 0x1b, 0x30, 0x99, 0x6d, 0x3a, 0x10, 0x35, 0x08, 0x58, 
        0x78, 0x9d, 0x07, 0x57, 0xb9, 0x09, 0x0c, 0x0e, 0x27, 0x1c, 0x03, 0xb0, 0xad, 0x97, 0x20, 0xf8, 
        0x67, 0x5e, 0x14, 0x17, 0xeb, 0x21, 0x10, 0xdd, 0x4d, 0xcd, 0x8f, 0xaa, 0x48, 0xe1, 0xd0, 0x6a, 
        0x7c, 0xac, 0x34, 0x21, 0x95, 0x00, 0x38, 0xb8, 0x17, 0xe3, 0xda, 0x6b, 0x12, 0x4c, 0x48, 0x2e, 
        0x1a, 0x7d, 0xd2, 0xc1, 0x6b, 0xa9, 0xc5, 0xf2, 0xba, 0xae, 0xd1, 0x2d, 0x6e, 0x07, 0x9b, 0x71, 
        0xdf, 0x4c, 0xca, 0xb2, 0xfc, 0x6f, 0x0f, 0x94, 0x04, 0x31, 0xb6, 0xe2, 0x1e, 0xcd, 0xcd, 0x0b, 
        0xf8, 0xbe, 0xbe, 0xf3, 0x88, 0x89, 0x89, 0xfc, 0xee, 0x39, 0x9f, 0x45, 0x33, 0x82, 0xb2, 0x21, 
        0x89, 0x75, 0x47, 0x0d, 0x0d, 0x1c, 0xdf, 0x59, 0xbe, 0x54, 0xab, 0xe7, 0xac, 0x86, 0x11, 0x04, 
        0xd1, 0x86, 0x72, 0xbe, 0x18, 0x56, 0xd4, 0x41, 0xa4, 0x7d, 0x48, 0xc4, 0x8b, 0x83, 0x21, 0x41, 
        0x9d, 0x6f, 0x3f, 0x84, 0xa9, 0x71, 0x8a, 0xce, 0xaa, 0x1f, 0x51, 0xcf, 0xac, 0x3c, 0x4c, 0xe3, 
        0xc4, 0x62, 0xd9, 0x4c, 0x38, 0xff, 0x75, 0x9f, 0xd9, 0xef, 0x5e, 0xd2, 0xc0, 0x11, 0x0b, 0x6f, 
        0x19, 0xca, 0x8b, 0x81, 0xd4, 0x2e, 0x06, 0x94, 0x80, 0xf7, 0x95, 0xc2, 0xe1, 0x86, 0x09, 0x87, 
        0x39, 0x31, 0x47, 0x84, 0xd7, 0xb3, 0x88, 0xb4, 0xaf, 0x98, 0x78, 0x2c, 0x2f, 0x41, 0xa4, 0x49, 
        0x0b, 0x14, 0x55, 0x31, 0x3b, 0xd1, 0x5e, 0xc8, 0x8d, 0x28, 0x38, 0x01, 0xa1, 0xbe, 0xa8, 0x60, 
        0xd7, 0x18, 0xd2, 0xe6, 0xab, 0x75, 0x84, 0xb1, 0x20, 0x04, 0x01, 0xcf, 0x08, 0x13, 0x35, 0x45, 
        0x59, 0xf4, 0xae, 0x53, 0x4c, 0xe4, 0xb0, 0xe2, 0x23, 0x10, 0x4f, 0x8b, 0x49, 0x13, 0x0c, 0x38, 
        0xed, 0x3c, 0x13, 0x46, 0x29, 0xed, 0xe2, 0xa1, 0xd3, 0x05, 0xe9, 0x51, 0x91, 0x48, 0x4e, 0xea, 
        0xa9, 0x24, 0xea, 0x7d, 0xb7, 0x2f, 0x60, 0x69, 0x13, 0x11, 0x42, 0xf6, 0xb5, 0x88, 0x2b, 0xd6, 
        0x9c, 0xda, 0x78, 0x48, 0xef, 0x98, 0xdc, 0x0c, 0x78, 0xed, 0xe0, 0x18, 0x62, 0x94, 0x7c, 0x85, 
        0x35, 0xe0, 0x60, 0x97, 0x84, 0xce, 0x51, 0x7b, 0x49, 0x52, 0x00, 0x7c, 0xaa, 0xe5, 0xa3, 0xd9, 
        0xbc, 0x90, 0xc2, 0x2a, 0xbc, 0x66, 0x92, 0x52, 0x21, 0xa5, 0xff, 0x12, 0xa9, 0xff, 0xf7, 0x75, 
        0xd0, 0x79, 0xbc, 0xa0, 0xfc, 0x86, 0x0f, 0x8b, 0x00, 0xcc, 0xb7, 0x6c, 0x80, 0x42, 0x10, 0x74, 
        0x71, 0x13, 0x08, 0xe8, 0x96, 0x09, 0x4b, 0x67, 0xc5, 0xf9, 0xff, 0xfc, 0xe7, 0x37, 0x09, 0xbf, 
};

// @1E017420
static int xcode_building_first_stage(struct cxdec_xcode_status *xcode)
{
      switch (xcode_rand(xcode) % 3) {
      case 2:
        // MOV ESI, EncryptionControlBlock
        // MOV EAX, DWORD PTR DS:[ESI+((xcode_rand(xcode) & 0x3ff) << 2)]
        if (!push_bytexcode(xcode, 0xbe)
                || !push_dwordxcode(xcode, (DWORD)EncryptionControlBlock)
                || !push_2bytesxcode(xcode, 0x8b, 0x86)
                || !push_dwordxcode(xcode, (xcode_rand(xcode) & 0x3ff) << 2))
            return 0;
        break;
    case 1:
        // MOV EAX, xcode_rand(xcode)
        if (!push_bytexcode(xcode, 0xb8)
                || !push_dwordxcode(xcode, xcode_rand(xcode)))
            return 0;
        break;
      case 0:
          // MOV EAX, EDI
          if (!push_2bytesxcode(xcode, 0x8b, 0xc7))
              return 0;
          break;
      }
      return 1;
}

// @1E017530
static int xcode_building_stage0(struct cxdec_xcode_status *xcode, int stage)
{
    if (stage == 1)
        return xcode_building_first_stage(xcode);

    if (xcode_rand(xcode) & 1) {
        if (!xcode_building_stage1(xcode, stage - 1))
            return 0;
    } else {
        if (!xcode_building_stage0(xcode, stage - 1))
            return 0;
    }

    switch (xcode_rand(xcode) & 7) {
    case 3:
        // NOT EAX
        if (!push_2bytesxcode(xcode, 0xf7, 0xd0))
            return 0;
        break;
    case 5:
        // DEC EAX
        if (!push_bytexcode(xcode, 0x48))
            return 0;
        break;
    case 7:
        // XOR EAX, xcode_rand(xcode)
        if (!push_bytexcode(xcode, 0x35) 
                || !push_dwordxcode(xcode, xcode_rand(xcode)))
            return 0;
        break;
    case 4:
        if (xcode_rand(xcode) & 1) {
            // ADD EAX, xcode_rand(xcode)
            if (!push_bytexcode(xcode, 0x05))
                return 0;
        } else {
            // SUB EAX, xcode_rand(xcode)
            if (!push_bytexcode(xcode, 0x2d))
                return 0;
        }
        if (!push_dwordxcode(xcode, xcode_rand(xcode)))
            return 0;
        break;
    case 2:
        // INC EAX
        if (!push_bytexcode(xcode, 0x40))
            return 0;
        break;
    case 1:
        // NEG EAX
        if (!push_2bytesxcode(xcode, 0xf7, 0xd8))
            return 0;
        break;
    case 6:
        // PUSH EBX
        // MOV EBX, EAX
        // AND EBX, AAAAAAAA
        // AND EAX, 55555555
        // SHR EBX, 1
        // SHL EAX, 1
        // OR EAX, EBX
        // POP EBX
        if (!push_bytexcode(xcode, 0x53)
                || !push_2bytesxcode(xcode, 0x89, 0xc3)
                || !push_6bytesxcode(xcode, 0x81, 0xe3, 0xaa, 0xaa, 0xaa, 0xaa)
                || !push_5bytesxcode(xcode, 0x25, 0x55, 0x55, 0x55, 0x55)
                || !push_2bytesxcode(xcode, 0xd1, 0xeb)
                || !push_2bytesxcode(xcode, 0xd1, 0xe0)
                || !push_2bytesxcode(xcode, 0x09, 0xd8)
                || !push_bytexcode(xcode, 0x5b))
            return 0;
        break;
    case 0:
        // MOV ESI, EncryptionControlBlock
        // AND EAX, 3FFh
        // MOV EAX, DWORD PTR DS:[ESI+EAX*4]
        if (!push_bytexcode(xcode, 0xbe)
                  || !push_dwordxcode(xcode, (DWORD)EncryptionControlBlock)
                || !push_bytexcode(xcode, 0x25)
                || !push_dwordxcode(xcode, 0x3ff)
                || !push_3bytesxcode(xcode, 0x8b, 0x04, 0x86))
            return 0;
        break;
    }
    return 1;
}

// @1E017790
static int xcode_building_stage1(struct cxdec_xcode_status *xcode, int stage)
{
    if (stage == 1)
        return xcode_building_first_stage(xcode);

    // PUSH EBX
    if (!push_bytexcode(xcode, 0x53))
        return 0;

    if (xcode_rand(xcode) & 1) {
        if (!xcode_building_stage1(xcode, stage - 1))
            return 0;
    } else {
        if (!xcode_building_stage0(xcode, stage - 1))
            return 0;
    }

    // MOV EBX, EAX
    if (!push_2bytesxcode(xcode, 0x89, 0xc3))
        return 0;

    if (xcode_rand(xcode) & 1) {
        if (!xcode_building_stage1(xcode, stage - 1))
            return 0;
    } else {
        if (!xcode_building_stage0(xcode, stage - 1))
            return 0;
    }

    switch (xcode_rand(xcode) % 6) {
    case 4:
        // ADD EAX, EBX
        if (!push_2bytesxcode(xcode, 0x01, 0xd8))
            return 0;
        break;
    case 3:
        // SUB EAX, EBX
        if (!push_2bytesxcode(xcode, 0x29, 0xd8))
            return 0;
        break;
    case 2:
        // PUSH ECX
        // MOV ECX, EBX
        // AND ECX, 0F
        // SHR EAX, CL
        // POP ECX
        if (!push_bytexcode(xcode, 0x51)
                || !push_2bytesxcode(xcode, 0x89, 0xd9)
                || !push_3bytesxcode(xcode, 0x83, 0xe1, 0x0f)
                || !push_2bytesxcode(xcode, 0xd3, 0xe8)
                || !push_bytexcode(xcode, 0x59))
            return 0;
        break;
    case 1:
        // IMUL EAX, EBX
        if (!push_3bytesxcode(xcode, 0x0f,  0xaf, 0xc3))
            return 0;
        break;
    case 5:
        // NEG EAX, ADD EAX, EBX
        if (!push_2bytesxcode(xcode, 0xf7, 0xd8) 
                || !push_2bytesxcode(xcode, 0x01, 0xd8))
            return 0;
        break;
    case 0:
        // PUSH ECX
        // MOV ECX, EBX
        // AND ECX, 0F
        // SHL EAX, CL
        // POP ECX
        if (!push_bytexcode(xcode, 0x51) 
                || !push_2bytesxcode(xcode, 0x89, 0xd9)
                || !push_3bytesxcode(xcode, 0x83, 0xe1, 0x0f)
                || !push_2bytesxcode(xcode, 0xd3, 0xe0)
                || !push_bytexcode(xcode, 0x59))
            return 0;
        break;
    }
    // POP EBX
    return push_bytexcode(xcode, 0x5b);
}

struct cxdec_callback silverblue_cxdec_callback = {
    "silverblue",
    { 0x1a1, 0x1eb },
    xcode_building_stage1
};
