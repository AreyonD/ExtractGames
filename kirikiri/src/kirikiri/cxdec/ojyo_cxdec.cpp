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
        0x82, 0xc4, 0x82, 0xa2, 0x82, 0xdc, 0x82, 0xb7, 0x81, 0x42, 0x0a, 0x8e, 0xa9, 0x91, 0xee, 0x82, 
        0xc9, 0x8b, 0x41, 0x82, 0xe9, 0x82, 0xc6, 0x96, 0x85, 0x82, 0xaa, 0x82, 0xc9, 0x82, 0xb1, 0x82, 
        0xc9, 0x82, 0xb1, 0x82, 0xb5, 0x82, 0xc4, 0x8f, 0x6f, 0x8c, 0x7d, 0x82, 0xa6, 0x82, 0xc4, 0x82, 
        0xad, 0x82, 0xea, 0x82, 0xe9, 0x81, 0x42, 0x0a, 0x81, 0x75, 0x8f, 0xac, 0x92, 0xb0, 0x82, 0xcc, 
        0xe0, 0x75, 0x94, 0xd1, 0x8b, 0x6c, 0x82, 0xdf, 0x82, 0xf0, 0x83, 0x5c, 0x81, 0x5b, 0x83, 0x5a, 
        0x81, 0x5b, 0x83, 0x57, 0x82, 0xcc, 0x97, 0x76, 0x97, 0xcc, 0x82, 0xc5, 0x82, 0xe2, 0x82, 0xea, 
        0x82, 0xce, 0x20, 0x6d, 0x70, 0x6c, 0x61, 0x79, 0x65, 0x72, 0x82, 0xaa, 0x8d, 0xc4, 0x90, 0xb6, 
        0x82, 0xb3, 0x82, 0xea, 0x81, 0x41, 0x91, 0xe5, 0x82, 0xcc, 0x8e, 0x9a, 0x82, 0xc9, 0x82, 0xc8, 
        0x82, 0xc1, 0x82, 0xbd, 0x8f, 0xf3, 0x91, 0xd4, 0x82, 0xc5, 0x82, 0xcc, 0x81, 0x41, 0x8f, 0x57, 
        0x92, 0x63, 0x82, 0xc8, 0x82, 0xf1, 0x82, 0xa9, 0x95, 0xcf, 0x82, 0xc8, 0x8a, 0xb4, 0x82, 0xb6, 
        0x82, 0xc9, 0x8b, 0xdf, 0x82, 0xa2, 0x8f, 0xf3, 0x91, 0xd4, 0x82, 0xc5, 0x82, 0xcd, 0x81, 0x5b, 
        0x81, 0x76, 0x0a, 0x91, 0x8a, 0x95, 0xcf, 0x82, 0xed, 0x82, 0xe7, 0x82, 0xb8, 0x89, 0xbd, 0x82, 
        0xf0, 0x8c, 0xbe, 0x82, 0xc1, 0x82, 0xc4, 0x82, 0xe9, 0x82, 0xcc, 0x82, 0xa9, 0x82, 0xed, 0x82, 
        0xa9, 0x82, 0xe7, 0x82, 0xc8, 0x82, 0xa2, 0x82, 0xaa, 0x81, 0x41, 0x82, 0xbd, 0x82, 0xd4, 0x82, 
        0xf1, 0x96, 0x6c, 0x82, 0xf0, 0x8a, 0xbd, 0x8c, 0x7d, 0x82, 0xb5, 0x82, 0xc4, 0x82, 0xad, 0x82, 
        0xea, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xe9, 0x82, 0xcc, 0x82, 0xbe, 0x82, 0xeb, 0x82, 0xa4, 0x81, 
        0x42, 0x94, 0xde, 0x8f, 0x97, 0x82, 0xcd, 0x82, 0xc8, 0x82, 0xc9, 0x82, 0xe7, 0x82, 0xe2, 0x82, 
        0xa4, 0x82, 0xea, 0x82, 0xb5, 0x82, 0xb0, 0x82, 0xc9, 0x81, 0x75, 0x82, 0xdf, 0x82, 0xe9, 0x82, 
        0xcd, 0x82, 0xcd, 0x82, 0xdf, 0x82, 0xcd, 0x82, 0xea, 0x82, 0xaf, 0x81, 0x4b, 0x82, 0xea, 0x82, 
        0xaf, 0x82, 0xea, 0x82, 0xaf, 0x82, 0xea, 0x82, 0xea, 0x82, 0xcd, 0x82, 0xea, 0x82, 0xcd, 0x82, 
        0xcd, 0x82, 0xea, 0x82, 0xdf, 0x82, 0xeb, 0x82, 0xdf, 0x82, 0xeb, 0x81, 0x76, 0x82, 0xc6, 0x8c, 
        0xbe, 0x82, 0xa2, 0x82, 0xc8, 0x82, 0xaa, 0x82, 0xe7, 0x81, 0x41, 0x96, 0x6c, 0x82, 0xcc, 0x8a, 
        0x93, 0x82, 0xc6, 0x8f, 0xe3, 0x92, 0x85, 0x82, 0xf0, 0x8e, 0xf3, 0x82, 0xaf, 0x8e, 0xe6, 0x82, 
        0xc1, 0x82, 0xbd, 0x81, 0x42, 0x0a, 0x0a, 0x94, 0xde, 0x8f, 0x97, 0x82, 0xaa, 0x82, 0xb1, 0x82, 
        0xa4, 0x82, 0xc8, 0x82, 0xc1, 0x82, 0xc4, 0x82, 0xb5, 0x82, 0xdc, 0x82, 0xc1, 0x82, 0xbd, 0x82, 
        0xcc, 0x82, 0xcd, 0x82, 0xbd, 0x82, 0xb5, 0x82, 0xa9, 0x88, 0xea, 0x83, 0x96, 0x8c, 0x8e, 0x91, 
        0x4f, 0x82, 0xbe, 0x82, 0xc1, 0x82, 0xbd, 0x82, 0xbe, 0x82, 0xeb, 0x82, 0xa4, 0x82, 0xa9, 0x81, 
        0x42, 0x96, 0x85, 0x82, 0xcd, 0x97, 0xbf, 0x97, 0x9d, 0x92, 0x86, 0x82, 0xc9, 0x82, 0xb1, 0x82, 
        0xf1, 0x82, 0xc8, 0x82, 0xb1, 0x82, 0xc6, 0x82, 0xf0, 0x8c, 0xfb, 0x91, 0x96, 0x82, 0xc1, 0x82, 
        0xbd, 0x81, 0x42, 0x0a, 0x81, 0x75, 0x8a, 0xae, 0x91, 0x53, 0x82, 0xc9, 0x97, 0xe2, 0x82, 0xdf, 
        0x82, 0xbd, 0x82, 0xe7, 0x81, 0x41, 0x8d, 0xbb, 0x93, 0x9c, 0x82, 0xf0, 0x20, 0x31, 0x35, 0x67, 
        0x89, 0xc1, 0x82, 0xa6, 0x82, 0xc4, 0x81, 0x41, 0x94, 0xbc, 0x95, 0xaa, 0x82, 0xad, 0x82, 0xe7, 
        0x82, 0xa2, 0x8d, 0xac, 0x82, 0xb4, 0x82, 0xc1, 0x82, 0xbd, 0x82, 0xc6, 0x82, 0xb1, 0x82, 0xeb, 
        0x82, 0xc5, 0x81, 0x41, 0xe0, 0x75, 0x94, 0xd1, 0x83, 0x54, 0x83, 0x93, 0x82, 0xcd, 0x81, 0x41, 
        0x82, 0xe0, 0x82, 0xa4, 0x93, 0xf1, 0x8e, 0x4f, 0x90, 0x6c, 0x82, 0xc5, 0x82, 0xb7, 0x81, 0x76, 
        0x0a, 0x96, 0x6c, 0x82, 0xcd, 0x82, 0xa0, 0x82, 0xed, 0x82, 0xc4, 0x82, 0xc4, 0x97, 0xbf, 0x97, 
        0x9d, 0x82, 0xf0, 0x82, 0xe2, 0x82, 0xdf, 0x82, 0xb3, 0x82, 0xb9, 0x81, 0x41, 0x94, 0xde, 0x8f, 
        0x97, 0x82, 0xf0, 0x8b, 0x78, 0x82, 0xdc, 0x82, 0xb9, 0x82, 0xbd, 0x81, 0x42, 0x96, 0x85, 0x82, 
        0xcd, 0x89, 0xbd, 0x8c, 0xcc, 0x8e, 0xa9, 0x95, 0xaa, 0x82, 0xaa, 0x97, 0xbf, 0x97, 0x9d, 0x82, 
        0xf0, 0x82, 0xe2, 0x82, 0xdf, 0x82, 0xb3, 0x82, 0xb9, 0x82, 0xe7, 0x82, 0xea, 0x82, 0xbd, 0x82, 
        0xcc, 0x82, 0xa9, 0x97, 0x9d, 0x89, 0xf0, 0x82, 0xc5, 0x82, 0xab, 0x82, 0xc8, 0x82, 0xa2, 0x82, 
        0xe6, 0x82, 0xa4, 0x82, 0xc8, 0x95, 0x5c, 0x8f, 0xee, 0x82, 0xbe, 0x82, 0xc1, 0x82, 0xbd, 0x82, 
        0xaa, 0x81, 0x41, 0x82, 0xbb, 0x82, 0xea, 0x88, 0xc8, 0x97, 0x88, 0x81, 0x41, 0x94, 0xde, 0x8f, 
        0x97, 0x82, 0xcc, 0x8c, 0xbe, 0x97, 0x74, 0x82, 0xcd, 0x97, 0x9d, 0x89, 0xf0, 0x95, 0x73, 0x94, 
        0x5c, 0x82, 0xc9, 0x82, 0xc8, 0x82, 0xc1, 0x82, 0xbd, 0x81, 0x42, 0x0a, 0x0a, 0x94, 0xde, 0x8f, 
        0x97, 0x82, 0xcd, 0x8c, 0xbe, 0x82, 0xc1, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xe9, 0x82, 0xb1, 0x82, 
        0xc6, 0x82, 0xaa, 0x88, 0xd3, 0x96, 0xa1, 0x95, 0x73, 0x96, 0xbe, 0x82, 0xc8, 0x82, 0xbe, 0x82, 
        0xaf, 0x82, 0xc5, 0x81, 0x41, 0x8e, 0x76, 0x8d, 0x6c, 0x8e, 0xa9, 0x91, 0xcc, 0x82, 0xcd, 0x82, 
        0xdc, 0x82, 0xc6, 0x82, 0xe0, 0x82, 0xc8, 0x82, 0xe6, 0x82, 0xa4, 0x82, 0xc8, 0x82, 0xcc, 0x82, 
        0xc5, 0x81, 0x41, 0x8d, 0xc5, 0x8b, 0xdf, 0x82, 0xcd, 0x82, 0xa0, 0x82, 0xdc, 0x82, 0xe8, 0x96, 
        0x6c, 0x82, 0xe0, 0x8b, 0x43, 0x82, 0xc9, 0x82, 0xb5, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xc8, 0x82, 
        0xa2, 0x81, 0x42, 0x82, 0xe0, 0x82, 0xc6, 0x82, 0xe0, 0x82, 0xc6, 0x88, 0xc0, 0x95, 0xa8, 0x82, 
        0xbe, 0x82, 0xa9, 0x82, 0xe7, 0x81, 0x41, 0x82, 0xc6, 0x82, 0xa2, 0x82, 0xa4, 0x82, 0xcc, 0x82, 
        0xe0, 0x82, 0xa0, 0x82, 0xe9, 0x81, 0x42, 0x82, 0xc6, 0x82, 0xc1, 0x82, 0xad, 0x82, 0xc9, 0x8e, 
        0xf5, 0x96, 0xbd, 0x82, 0xcd, 0x89, 0xdf, 0x82, 0xac, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xe9, 0x82, 
        0xed, 0x82, 0xaf, 0x82, 0xbe, 0x82, 0xb5, 0x81, 0x41, 0x97, 0xbf, 0x97, 0x9d, 0x90, 0xea, 0x96, 
        0xe5, 0x82, 0xcc, 0x83, 0x41, 0x83, 0x93, 0x83, 0x68, 0x83, 0x8d, 0x83, 0x43, 0x83, 0x68, 0x82, 
        0xc9, 0x89, 0xc6, 0x8e, 0x96, 0x91, 0x53, 0x94, 0xca, 0x82, 0xf0, 0x82, 0xe2, 0x82, 0xe7, 0x82, 
        0xb9, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xe9, 0x82, 0xf1, 0x82, 0xbe, 0x82, 0xa9, 0x82, 0xe7, 0x89, 
        0xdf, 0x95, 0x89, 0x89, 0xd7, 0x82, 0xaa, 0x82, 0xa9, 0x82, 0xa9, 0x82, 0xc1, 0x82, 0xc4, 0x82, 
        0xe9, 0x82, 0xcc, 0x82, 0xcd, 0x95, 0xaa, 0x82, 0xa9, 0x82, 0xc1, 0x82, 0xc4, 0x82, 0xa2, 0x82, 
        0xe9, 0x81, 0x42, 0x0a, 0x0a, 0x82, 0xc6, 0x82, 0xcd, 0x8c, 0xbe, 0x82, 0xc1, 0x82, 0xc4, 0x82, 
        0xe0, 0x81, 0x63, 0x81, 0x63, 0x81, 0x63, 0x81, 0x42, 0x97, 0xbf, 0x97, 0x9d, 0x92, 0x86, 0x82, 
        0xc9, 0x81, 0x75, 0x82, 0xc5, 0x82, 0xe0, 0x8e, 0xe8, 0x82, 0xe0, 0x83, 0x6c, 0x83, 0x6f, 0x83, 
        0x6c, 0x83, 0x6f, 0x81, 0x60, 0x82, 0xc2, 0x82, 0xa2, 0x82, 0xc5, 0x82, 0xc9, 0x8e, 0x43, 0x82, 
        0xc1, 0x82, 0xbd, 0x83, 0x4d, 0x83, 0x55, 0x83, 0x4d, 0x83, 0x55, 0x83, 0x74, 0x83, 0x89, 0x83, 
        0x43, 0x83, 0x70, 0x83, 0x93, 0x82, 0xe0, 0x83, 0x6c, 0x83, 0x6f, 0x83, 0x6c, 0x83, 0x6f, 0x81, 
        0x60, 0x82, 0xc2, 0x82, 0xa2, 0x82, 0xc5, 0x82, 0xc9, 0x8e, 0x43, 0x82, 0xc1, 0x82, 0xbd, 0x83, 
        0x4d, 0x83, 0x55, 0x83, 0x4d, 0x83, 0x55, 0x83, 0x74, 0x83, 0x89, 0x83, 0x43, 0x83, 0x70, 0x83, 
        0x93, 0x82, 0xe0, 0x83, 0x6c, 0x83, 0x6f, 0x83, 0x6c, 0x83, 0x6f, 0x81, 0x60, 0x82, 0xc2, 0x82, 
        0xa2, 0x82, 0xc5, 0x82, 0xc9, 0x8e, 0x43, 0x82, 0xc1, 0x82, 0xbd, 0x83, 0x4d, 0x83, 0x55, 0x83, 
        0x4d, 0x83, 0x55, 0x83, 0x74, 0x83, 0x89, 0x83, 0x43, 0x83, 0x70, 0x83, 0x93, 0x82, 0xe0, 0x83, 
        0x6c, 0x83, 0x6f, 0x83, 0x6c, 0x83, 0x6f, 0x81, 0x60, 0x82, 0xc2, 0x82, 0xa2, 0x82, 0xc5, 0x82, 
        0xc9, 0x8e, 0x43, 0x82, 0xc1, 0x82, 0xbd, 0x83, 0x4d, 0x83, 0x55, 0x83, 0x4d, 0x83, 0x55, 0x83, 
        0x74, 0x83, 0x89, 0x83, 0x43, 0x83, 0x70, 0x83, 0x93, 0x82, 0xe0, 0x83, 0x6c, 0x83, 0x6f, 0x83, 
        0x6c, 0x83, 0x6f, 0x81, 0x60, 0x82, 0xc2, 0x82, 0xa2, 0x82, 0xc5, 0x82, 0xc9, 0x8e, 0x43, 0x82, 
        0xc1, 0x82, 0xbd, 0x83, 0x4d, 0x83, 0x55, 0x83, 0x4d, 0x83, 0x55, 0x83, 0x74, 0x83, 0x89, 0x83, 
        0x43, 0x83, 0x70, 0x83, 0x93, 0x82, 0xe0, 0x83, 0x6c, 0x83, 0x6f, 0x83, 0x6c, 0x83, 0x6f, 0x81, 
        0x60, 0x82, 0xc2, 0x82, 0xa2, 0x82, 0xc5, 0x82, 0xc9, 0x8e, 0x43, 0x82, 0xc1, 0x82, 0xbd, 0x83, 
        0x4d, 0x83, 0x55, 0x83, 0x4d, 0x83, 0x55, 0x83, 0x74, 0x83, 0x89, 0x83, 0x43, 0x83, 0x70, 0x83, 
        0x93, 0x82, 0xe0, 0x83, 0x6c, 0x83, 0x6f, 0x83, 0x6c, 0x83, 0x6f, 0x81, 0x60, 0x82, 0xc2, 0x82, 
        0xa2, 0x82, 0xc5, 0x82, 0xc9, 0x8e, 0x43, 0x82, 0xc1, 0x82, 0xbd, 0x83, 0x4d, 0x83, 0x55, 0x83, 
        0x4d, 0x83, 0x55, 0x83, 0x74, 0x83, 0x89, 0x83, 0x43, 0x83, 0x70, 0x83, 0x93, 0x82, 0xe0, 0x83, 
        0x6c, 0x83, 0x6f, 0x83, 0x6c, 0x83, 0x6f, 0x81, 0x60, 0x82, 0xc2, 0x82, 0xa2, 0x82, 0xc5, 0x82, 
        0xc9, 0x8e, 0x43, 0x82, 0xc1, 0x82, 0xbd, 0x83, 0x4d, 0x83, 0x55, 0x83, 0x4d, 0x83, 0x55, 0x83, 
        0x74, 0x83, 0x89, 0x83, 0x43, 0x83, 0x70, 0x83, 0x93, 0x82, 0xe0, 0x83, 0x6c, 0x83, 0x6f, 0x83, 
        0x6c, 0x83, 0x6f, 0x81, 0x60, 0x81, 0x76, 0x82, 0xc6, 0x82, 0xa9, 0x95, 0x40, 0x89, 0xcc, 0x8d, 
        0xac, 0x82, 0xb6, 0x82, 0xe8, 0x82, 0xc5, 0x89, 0xcc, 0x82, 0xed, 0x82, 0xea, 0x82, 0xe9, 0x82, 
        0xc6, 0x81, 0x41, 0x82, 0xa2, 0x82, 0xc1, 0x82, 0xbd, 0x82, 0xa2, 0x89, 0xbd, 0x82, 0xf0, 0x90, 
        0x48, 0x82, 0xed, 0x82, 0xb3, 0x82, 0xea, 0x82, 0xe9, 0x82, 0xcc, 0x82, 0xa9, 0x94, 0xf1, 0x8f, 
        0xed, 0x82, 0xc9, 0x95, 0x73, 0x88, 0xc0, 0x82, 0xc9, 0x82, 0xc8, 0x82, 0xe9, 0x81, 0x42, 0x0a, 
        0x0a, 0x72, 0x6c, 0x06, 0x16, 0x28, 0xc1, 0xc8, 0x3c, 0x73, 0x83, 0xdd, 0xc0, 0x4a, 0x6d, 0x63, 
        0x9c, 0x0c, 0xb1, 0xfe, 0xb3, 0x4f, 0x3a, 0x3a, 0x5c, 0x8d, 0xc5, 0xd1, 0xf1, 0x4f, 0x31, 0x7a, 
        0x2f, 0xae, 0xc5, 0x2d, 0x94, 0x2c, 0x6c, 0x64, 0xff, 0x77, 0x35, 0x9d, 0x16, 0x39, 0xd6, 0x18, 
        0x4d, 0xbe, 0xc4, 0xb7, 0xe5, 0x4a, 0x41, 0x54, 0x91, 0x6c, 0x25, 0x59, 0x4d, 0xa7, 0x59, 0xc2, 
        0x8a, 0xc9, 0x92, 0xbf, 0x85, 0x47, 0x9c, 0xe1, 0xb0, 0x37, 0xbf, 0x7b, 0x66, 0xed, 0x29, 0xff, 
        0xd2, 0x74, 0xa1, 0x6c, 0x0d, 0x4a, 0xff, 0xef, 0x60, 0xc1, 0xc7, 0xd9, 0x82, 0xba, 0x3f, 0xb4, 
        0x24, 0x25, 0x3e, 0xac, 0x14, 0xcd, 0xf7, 0x3b, 0xdb, 0xf9, 0xa9, 0x5b, 0x44, 0x79, 0x08, 0xd7, 
        0x19, 0x76, 0xb4, 0x3e, 0x5e, 0x04, 0x8d, 0x7b, 0x7a, 0xed, 0x20, 0x8a, 0xe2, 0x93, 0x65, 0x93, 
        0x50, 0xc0, 0x9e, 0xd6, 0x69, 0x39, 0x41, 0x35, 0xd6, 0x2d, 0x7f, 0xc1, 0xff, 0x2f, 0x60, 0x13, 
        0x7a, 0x03, 0x86, 0x1e, 0xb9, 0x32, 0x11, 0x58, 0xea, 0xa1, 0xac, 0x81, 0xdd, 0x58, 0x16, 0xcc, 
        0xd0, 0xb4, 0x67, 0x3f, 0x9b, 0x2d, 0xc7, 0x2e, 0xa4, 0x98, 0xff, 0xe8, 0x17, 0xed, 0x32, 0x3b, 
        0x75, 0x8a, 0x67, 0xcd, 0x86, 0xc9, 0xd1, 0x0c, 0xd5, 0xf6, 0x6d, 0xfd, 0xa3, 0x71, 0xcc, 0x2b, 
        0x48, 0xfa, 0x2a, 0xa9, 0x80, 0xae, 0xa6, 0x80, 0xb7, 0x21, 0xe5, 0x20, 0x1a, 0x73, 0x2a, 0xbd, 
        0x69, 0xcc, 0x48, 0x61, 0x87, 0xff, 0xb4, 0xa6, 0xc8, 0xb6, 0x0f, 0x93, 0xc9, 0xd4, 0x94, 0x0a, 
        0x3e, 0x36, 0x16, 0xd9, 0x90, 0x3f, 0x18, 0x37, 0x52, 0xf1, 0x06, 0x9d, 0xad, 0x72, 0xdf, 0x51, 
        0xf2, 0xa0, 0x79, 0x1b, 0xe4, 0xd7, 0xec, 0x17, 0xc0, 0x9f, 0x88, 0x7d, 0x43, 0x2d, 0x33, 0x48, 
        0xf6, 0x48, 0xbf, 0xc7, 0xaa, 0x39, 0x90, 0x5b, 0x48, 0x28, 0x7c, 0x04, 0x77, 0x9b, 0xe9, 0xcd, 
        0x48, 0x23, 0x00, 0x46, 0x2b, 0x89, 0x12, 0xb7, 0xba, 0xe0, 0x6c, 0x32, 0xda, 0x21, 0xc1, 0xc2, 
        0xce, 0xeb, 0x24, 0xfb, 0x62, 0x08, 0x5a, 0x89, 0xe2, 0xe8, 0x33, 0xf5, 0xba, 0xf0, 0x16, 0x17, 
        0xb7, 0x8b, 0x69, 0xd8, 0x8e, 0xec, 0x68, 0xc3, 0xf4, 0x84, 0x30, 0x2d, 0x71, 0xfa, 0x04, 0x0d, 
        0x79, 0x0a, 0xc1, 0xcd, 0xb9, 0x9a, 0x9a, 0x9d, 0xa4, 0x79, 0x7c, 0x4b, 0xd6, 0x8b, 0xf8, 0x74, 
        0x1f, 0x62, 0xa3, 0x2f, 0x96, 0x38, 0x8c, 0x0e, 0x43, 0x19, 0x2d, 0xfb, 0x54, 0xe3, 0xc6, 0xb1, 
        0xc3, 0x31, 0xdf, 0x5e, 0x63, 0xbe, 0xc0, 0xba, 0xac, 0xc4, 0x7d, 0x59, 0xeb, 0xae, 0x95, 0x83, 
        0x16, 0x68, 0x55, 0x04, 0x8e, 0x4e, 0xd6, 0x9d, 0x98, 0x4f, 0x08, 0xf3, 0x47, 0x0e, 0x6b, 0xab, 
        0xa7, 0x30, 0x6d, 0x69, 0xe6, 0x2b, 0x5c, 0x51, 0x78, 0xde, 0x35, 0x5e, 0xf2, 0xc6, 0x70, 0x14, 
        0x7d, 0xd7, 0xcf, 0x7a, 0xec, 0x58, 0xa4, 0xfe, 0x92, 0xca, 0xdc, 0x40, 0x27, 0xf4, 0xfc, 0x6e, 
        0x76, 0xd2, 0x9c, 0xcc, 0xd3, 0x00, 0xf5, 0x91, 0xd0, 0xf6, 0x52, 0x7c, 0x6d, 0xba, 0xaf, 0x20, 
        0xc8, 0x55, 0x59, 0x6a, 0xeb, 0x6c, 0x4d, 0x0d, 0xc5, 0xeb, 0xd3, 0x07, 0x4f, 0xf4, 0xfa, 0xa8, 
        0x4d, 0xd7, 0xff, 0x79, 0xdc, 0x67, 0xaf, 0xd4, 0x02, 0xcb, 0x1b, 0x5f, 0x93, 0xab, 0x9a, 0xad, 
        0xb9, 0x46, 0x38, 0xc6, 0xa2, 0x8c, 0x8c, 0x6c, 0x0d, 0x4e, 0xf1, 0xe9, 0x08, 0x8f, 0x9f, 0x08, 
        0xa1, 0xf3, 0xf0, 0x8c, 0xf8, 0xb0, 0xfa, 0x70, 0x51, 0x02, 0xb2, 0x67, 0x91, 0xfc, 0xc0, 0xca, 
        0xc1, 0x48, 0x85, 0x3a, 0x54, 0xa2, 0x07, 0x6d, 0x19, 0x48, 0x59, 0x9f, 0x3a, 0x86, 0x6a, 0x49, 
        0x11, 0x91, 0xf9, 0xd2, 0xed, 0x96, 0x95, 0x10, 0x33, 0x92, 0x67, 0x07, 0x44, 0xd5, 0xf0, 0xf3, 
        0x60, 0x15, 0x35, 0xe2, 0x34, 0x5f, 0x55, 0xb2, 0xfc, 0x8b, 0xc4, 0x6f, 0xb3, 0x12, 0x1e, 0x3b, 
        0x52, 0x25, 0x81, 0x76, 0x65, 0x71, 0x95, 0xe2, 0xc1, 0xfe, 0xe2, 0xa0, 0x91, 0x63, 0xce, 0x14, 
        0x27, 0x0a, 0xfe, 0xc2, 0x68, 0x97, 0x21, 0xb9, 0x79, 0x51, 0x67, 0xbd, 0xa7, 0x80, 0x7f, 0x15, 
        0x6a, 0x3d, 0x82, 0x1c, 0x65, 0x1b, 0x3f, 0x59, 0xf5, 0xb6, 0x62, 0x17, 0xa8, 0x9b, 0xce, 0xf6, 
        0x38, 0x16, 0xe8, 0xd1, 0xfe, 0x79, 0x40, 0x22, 0x54, 0x30, 0x9c, 0x79, 0x46, 0x66, 0x90, 0xc5, 
        0x76, 0x19, 0xfb, 0x24, 0x58, 0xbe, 0xe9, 0x41, 0x19, 0x39, 0xca, 0x69, 0x74, 0xb4, 0xbd, 0xc3, 
        0xeb, 0xcf, 0x39, 0xc7, 0x1f, 0x47, 0xf7, 0x62, 0xe6, 0x71, 0x6c, 0x9f, 0x11, 0xba, 0x89, 0x01, 
        0xc8, 0x8e, 0x6e, 0x58, 0xf5, 0xd7, 0x30, 0x65, 0x0b, 0x63, 0x4f, 0xef, 0xee, 0x2b, 0xb8, 0xf7, 
        0x6a, 0xec, 0x8f, 0x79, 0xb6, 0xef, 0xa5, 0x2b, 0xa5, 0x90, 0x5f, 0x1d, 0x3e, 0x86, 0xc8, 0x62, 
        0x8d, 0x6b, 0x21, 0x45, 0x3c, 0xa4, 0x2f, 0x88, 0x2a, 0x00, 0xeb, 0xaa, 0x98, 0x79, 0xac, 0xef, 
        0x7d, 0xeb, 0x57, 0x7f, 0xd4, 0x1f, 0xa3, 0xc5, 0x7d, 0x43, 0xb9, 0x49, 0x5e, 0xfc, 0x05, 0xb7, 
        0x4c, 0xa9, 0xe0, 0x7e, 0xcb, 0xbc, 0x8a, 0x6e, 0x19, 0x80, 0x72, 0x41, 0xe8, 0xce, 0x46, 0x57, 
        0x45, 0x17, 0x6d, 0x68, 0x07, 0xb8, 0x07, 0x1c, 0x9e, 0xa2, 0x66, 0xba, 0x14, 0x26, 0x7f, 0x63, 
        0x71, 0x99, 0x88, 0x8c, 0xcc, 0x21, 0x87, 0xbc, 0xaf, 0x80, 0x74, 0x79, 0x52, 0x0d, 0x70, 0x42, 
        0x27, 0x33, 0x9f, 0xae, 0xee, 0x43, 0x1c, 0x3c, 0x90, 0xdf, 0x65, 0x36, 0x0b, 0xf6, 0x62, 0x7a, 
        0xab, 0x66, 0x91, 0xc3, 0xbe, 0x73, 0x6c, 0xe7, 0x40, 0x64, 0x36, 0x6c, 0x40, 0xc0, 0x30, 0x22, 
        0xaa, 0x99, 0xd6, 0x42, 0x45, 0x3e, 0x41, 0x07, 0x43, 0x9a, 0xe4, 0x7a, 0x7b, 0x78, 0x2f, 0x61, 
        0x51, 0x8e, 0x64, 0xc5, 0xd1, 0x9d, 0xf2, 0x76, 0x05, 0xd0, 0xff, 0xb8, 0x33, 0xcf, 0x2a, 0xdb, 
        0x82, 0xed, 0x9f, 0xd9, 0x27, 0xf0, 0x74, 0x54, 0xe2, 0xc5, 0xcb, 0x23, 0x78, 0xd8, 0xdf, 0xb2, 
        0x2b, 0xb1, 0x30, 0x75, 0x21, 0xa7, 0xfe, 0x5b, 0xe9, 0x2d, 0x79, 0xec, 0x4b, 0x34, 0x73, 0xc6, 
        0x0f, 0xc4, 0xde, 0xc7, 0x4b, 0x11, 0x52, 0xc8, 0xac, 0xa8, 0x01, 0xe9, 0xbe, 0x15, 0x81, 0x1c, 
        0x36, 0xd6, 0x82, 0x85, 0x05, 0x00, 0x68, 0x93, 0x77, 0x0f, 0x66, 0xe4, 0x73, 0x6e, 0x08, 0xc9, 
        0x8c, 0xed, 0xe3, 0xa5, 0xf9, 0xfe, 0xb5, 0x1e, 0x00, 0xec, 0xa8, 0xfd, 0xd7, 0x05, 0x13, 0x90, 
        0x45, 0x89, 0x68, 0x81, 0x36, 0x70, 0x13, 0x68, 0x22, 0x2e, 0xf0, 0xf3, 0x7f, 0x72, 0xa0, 0xdc, 
        0x2a, 0x9e, 0x22, 0x7e, 0x74, 0xba, 0xce, 0xa3, 0xf9, 0xd2, 0x83, 0xb9, 0xb9, 0x81, 0xdf, 0x6a, 
        0x34, 0xed, 0x56, 0x78, 0x13, 0x75, 0x0b, 0xf9, 0xb7, 0x30, 0xbe, 0x73, 0x6e, 0xd8, 0x18, 0xab, 
        0xe3, 0x4d, 0xd4, 0x4e, 0x23, 0x83, 0xde, 0xa3, 0xbe, 0x6a, 0xdf, 0xf2, 0xb8, 0xec, 0x09, 0xdb, 
        0xe1, 0xa3, 0x07, 0x93, 0x3b, 0x5d, 0xf4, 0x5b, 0xa4, 0x8e, 0xb2, 0x9e, 0x8c, 0x10, 0xc2, 0x9b, 
        0xb4, 0x6a, 0x37, 0x80, 0xaf, 0xad, 0xe9, 0x0b, 0xdb, 0xf6, 0xa1, 0x9d, 0x7b, 0xd6, 0xa6, 0x58, 
        0x3f, 0x30, 0xb4, 0x5a, 0xbb, 0x3d, 0x02, 0x09, 0x6e, 0x55, 0x6a, 0x5c, 0x4f, 0x48, 0x47, 0xf4, 
        0x16, 0xc4, 0x35, 0xc2, 0xd7, 0x16, 0x06, 0x7a, 0x53, 0xc8, 0x56, 0x18, 0xb9, 0x81, 0x85, 0x50, 
        0x36, 0x2b, 0x3d, 0xda, 0x1e, 0x23, 0xc1, 0xeb, 0x76, 0xf2, 0xe0, 0x4a, 0x0a, 0x5a, 0x6d, 0xe9, 
        0x54, 0xd2, 0x32, 0x5e, 0xbf, 0xde, 0x29, 0x07, 0x3c, 0x5e, 0xf0, 0x46, 0x81, 0x04, 0x7b, 0x5b, 
        0x14, 0x70, 0x24, 0x02, 0x8e, 0x2e, 0xe9, 0x28, 0x70, 0xde, 0xd9, 0xbf, 0xd4, 0x75, 0x78, 0x47, 
        0x23, 0x23, 0x3a, 0x5b, 0x7c, 0xb3, 0x14, 0xf9, 0x11, 0xe6, 0xf8, 0xc3, 0x4c, 0x4c, 0xe7, 0xb1, 
        0x4b, 0x1f, 0x04, 0x63, 0x4f, 0xdf, 0x7b, 0x15, 0xae, 0xda, 0xeb, 0x0a, 0x54, 0xc8, 0x65, 0x05, 
        0x22, 0xb1, 0x58, 0x2f, 0x8f, 0xee, 0x31, 0xb2, 0x8e, 0x06, 0xf8, 0x81, 0x92, 0xbc, 0x39, 0xa7, 
        0xc8, 0x1c, 0x53, 0x19, 0xd4, 0x4c, 0xee, 0xeb, 0x91, 0x07, 0x59, 0xa8, 0xb2, 0x63, 0x42, 0xf7, 
        0x8a, 0x67, 0xcc, 0x36, 0x1f, 0xb9, 0xa1, 0x09, 0xc4, 0x8f, 0xd4, 0x6b, 0xad, 0xa9, 0x2c, 0x67, 
        0x13, 0xd8, 0x22, 0x23, 0x2d, 0x89, 0x28, 0x98, 0x58, 0x10, 0x7f, 0x3c, 0x7a, 0x22, 0x66, 0x5e, 
        0x53, 0xf9, 0xea, 0xb7, 0xad, 0x04, 0x30, 0x60, 0xb4, 0xf2, 0x60, 0xf5, 0xbf, 0x78, 0x86, 0xae, 
        0xfe, 0x1c, 0xa8, 0xe6, 0x8a, 0x82, 0x01, 0xe1, 0x14, 0xae, 0x95, 0xf6, 0x9a, 0x4d, 0x44, 0x26, 
        0x84, 0xf8, 0xdd, 0x30, 0xf8, 0x11, 0x77, 0x9d, 0x29, 0xa1, 0xdf, 0xbd, 0xe8, 0x18, 0x0e, 0xcd, 
        0xae, 0xa4, 0xcd, 0x14, 0x1c, 0xfb, 0x19, 0x90, 0xf5, 0x3f, 0x08, 0x41, 0xd8, 0xbc, 0x94, 0xfc, 
        0x7b, 0x3c, 0x0d, 0x30, 0x63, 0xbe, 0x6e, 0xe5, 0x0e, 0x2e, 0xc1, 0x03, 0xfc, 0x9e, 0x84, 0x2f, 
        0x2c, 0xb6, 0xfb, 0xc4, 0x07, 0x03, 0xaf, 0x81, 0xc1, 0xb8, 0xa6, 0x60, 0xe0, 0x38, 0xa6, 0x09, 
        0xdc, 0x7e, 0x6b, 0x7b, 0x91, 0xef, 0x29, 0xbf, 0x0d, 0x5c, 0x50, 0x4a, 0x0a, 0xb7, 0x4c, 0x17, 
        0xc9, 0x09, 0x73, 0x75, 0xb4, 0x9a, 0x51, 0xb1, 0x22, 0x28, 0xde, 0x4e, 0xca, 0x56, 0x81, 0x93, 
        0xf9, 0xd5, 0xdd, 0xb6, 0x72, 0x3e, 0xfe, 0xf2, 0x24, 0x6a, 0x14, 0xbf, 0xe2, 0xc7, 0xa3, 0x72, 
        0x13, 0xa9, 0xe6, 0xb4, 0x37, 0x3d, 0x93, 0x33, 0x19, 0x97, 0xd2, 0x18, 0x3d, 0x1d, 0x2e, 0x40, 
        0xe4, 0xd9, 0x97, 0x5f, 0x94, 0xb7, 0x5b, 0xcb, 0x05, 0x4e, 0xa6, 0x51, 0x6e, 0xd6, 0x05, 0xd9, 
        0x9b, 0xa8, 0x18, 0x32, 0x0f, 0xa5, 0x0d, 0x25, 0x57, 0x76, 0x87, 0x68, 0x01, 0x1b, 0x55, 0xb4, 
        0xf9, 0xb1, 0x11, 0xd0, 0x67, 0xe6, 0x88, 0xa0, 0x70, 0xb3, 0x43, 0x90, 0x89, 0x29, 0x15, 0x21, 
        0x4a, 0x56, 0x3b, 0x89, 0x81, 0xf6, 0xf1, 0x81, 0x27, 0xec, 0xde, 0x97, 0x37, 0xbf, 0x7c, 0xdd, 
        0x8f, 0x3f, 0x65, 0x8c, 0x33, 0xf9, 0x15, 0x3d, 0x4c, 0x5e, 0xc3, 0xb1, 0x29, 0x30, 0x40, 0xad, 
        0x06, 0x39, 0xde, 0x5e, 0x42, 0x1a, 0xe5, 0x03, 0xcd, 0x32, 0xa8, 0x72, 0x79, 0x3b, 0x20, 0x19, 
        0xa2, 0x3a, 0x4e, 0x17, 0x96, 0x24, 0x37, 0x8c, 0xca, 0xd6, 0xd8, 0xed, 0x85, 0x99, 0x5d, 0x3b, 
        0xec, 0x94, 0x16, 0xbf, 0xc7, 0x8a, 0xeb, 0xda, 0xf3, 0xcd, 0x8c, 0x7d, 0x25, 0x92, 0x67, 0x00, 
        0xa6, 0x6c, 0x14, 0x98, 0xac, 0x9d, 0x4b, 0x28, 0x3b, 0xfb, 0x2f, 0x73, 0x95, 0xc0, 0x0a, 0x7f, 
        0x54, 0xae, 0x2b, 0xdb, 0xe4, 0x47, 0x17, 0x71, 0x81, 0xd9, 0x84, 0xeb, 0x0e, 0x32, 0xbb, 0x16, 
        0x55, 0xd8, 0x10, 0x84, 0xb7, 0x65, 0x4c, 0x23, 0x74, 0x58, 0x50, 0x3d, 0x85, 0x37, 0x04, 0xb1, 
        0x6b, 0xa1, 0x92, 0xee, 0x87, 0xc2, 0xd3, 0x87, 0x96, 0x2b, 0xae, 0x99, 0xd5, 0xcb, 0xf0, 0x97, 
        0x1f, 0x03, 0x9f, 0x97, 0xfb, 0x9c, 0x74, 0xc7, 0x67, 0xe8, 0x75, 0xf9, 0x84, 0xb9, 0xbb, 0x71, 
        0x19, 0xf4, 0x66, 0x96, 0x49, 0x70, 0x6b, 0x5b, 0xd7, 0x8e, 0xab, 0xbc, 0xaf, 0x9f, 0xd8, 0x2b, 
        0x9a, 0xa4, 0xb5, 0xd5, 0x18, 0xaa, 0x47, 0xfd, 0xc5, 0x15, 0x09, 0xd1, 0x0e, 0xaf, 0x68, 0x19, 
        0x7c, 0x33, 0x21, 0x53, 0x9b, 0x50, 0xa9, 0xae, 0xee, 0xd3, 0xe0, 0x0d, 0xcf, 0x14, 0xe4, 0xbd, 
        0x96, 0x01, 0x54, 0x55, 0x99, 0xec, 0xcc, 0x33, 0xda, 0x29, 0x29, 0x4b, 0x07, 0x9c, 0x56, 0x63, 
        0xaf, 0xd4, 0xe3, 0x6e, 0xcb, 0x1b, 0x3a, 0xd0, 0xdc, 0x74, 0xbe, 0x31, 0xb8, 0xc7, 0x2b, 0x30, 
        0xa6, 0x28, 0x2c, 0x1a, 0x7c, 0x3b, 0x37, 0x68, 0x72, 0x7a, 0xd2, 0x04, 0xda, 0x23, 0x32, 0x87, 
        0xa6, 0x24, 0xf8, 0x8d, 0x1c, 0x68, 0x27, 0x86, 0x71, 0x96, 0xe4, 0x17, 0xbc, 0xd5, 0x8f, 0x3c, 
        0x60, 0x58, 0x96, 0xb1, 0xf0, 0x7b, 0x32, 0x35, 0xf0, 0x8d, 0x83, 0x07, 0xe0, 0xee, 0x8d, 0x1e, 
        0x57, 0xf2, 0xe2, 0x33, 0xc0, 0xfa, 0x92, 0x7d, 0x28, 0x9b, 0x58, 0x12, 0xe0, 0x31, 0x34, 0x27, 
        0x80, 0x4b, 0x40, 0x5e, 0x11, 0xf7, 0x27, 0x86, 0x7a, 0x66, 0x9a, 0xf9, 0x54, 0xb1, 0x64, 0x88, 
        0x75, 0xc9, 0x2b, 0x66, 0xe2, 0x7d, 0x82, 0x19, 0x9c, 0x8d, 0x12, 0x72, 0xeb, 0x25, 0x9e, 0x28, 
        0x48, 0xa8, 0xb3, 0x93, 0x59, 0xd4, 0x23, 0x3d, 0x51, 0x5d, 0x1f, 0xea, 0x8b, 0x0f, 0xe0, 0x16, 
        0xf9, 0x6d, 0xb7, 0x45, 0x0c, 0x77, 0xff, 0xca, 0x65, 0x9c, 0x99, 0xd2, 0xd7, 0x67, 0x26, 0xa9, 
        0xc9, 0x5b, 0x83, 0x6f, 0x09, 0xc8, 0xb1, 0x15, 0xb5, 0xa0, 0x44, 0xf5, 0xb5, 0x9a, 0x90, 0xb3, 
        0x43, 0x5f, 0x5e, 0xc7, 0x25, 0x1c, 0xb6, 0x3b, 0x8e, 0xfc, 0x78, 0x60, 0x75, 0x58, 0x10, 0x26, 
        0xe2, 0x96, 0x63, 0x8b, 0x4e, 0x65, 0xdd, 0x6e, 0x89, 0x2b, 0xe0, 0x83, 0x6c, 0x32, 0xe5, 0xd5, 
        0x80, 0xa4, 0x89, 0x5c, 0xcb, 0xde, 0x6f, 0x94, 0x71, 0xc9, 0xc8, 0xe8, 0xf2, 0x52, 0xf7, 0xfb, 
        0xa7, 0xb4, 0xdc, 0xcb, 0x58, 0x35, 0x24, 0x4f, 0x77, 0xfb, 0x14, 0xe2, 0x43, 0x07, 0x26, 0xe8, 
        0x2c, 0x1c, 0x17, 0xcb, 0xbf, 0xe2, 0x85, 0x8b, 0xb2, 0xd7, 0x0d, 0x4c, 0x04, 0x2d, 0xd9, 0x45, 
        0x03, 0x46, 0xe7, 0x84, 0x30, 0xdd, 0x96, 0x5a, 0x4c, 0x38, 0xef, 0x7d, 0xa2, 0xf2, 0x23, 0x84, 
        0x5c, 0x12, 0xbc, 0xe4, 0x2c, 0x27, 0xba, 0x8a, 0x1d, 0x03, 0xa3, 0x45, 0x86, 0xbd, 0x84, 0xb6, 
        0x44, 0x88, 0xb1, 0x24, 0x85, 0x17, 0x5a, 0x71, 0xe4, 0x8d, 0x40, 0xe8, 0x2b, 0x72, 0x66, 0x21, 
        0x1d, 0xe0, 0x35, 0xf1, 0x0b, 0x1c, 0x87, 0x25, 0x1b, 0x69, 0x8c, 0x93, 0xb6, 0x1e, 0x96, 0x7f, 
        0xd9, 0x74, 0xbf, 0xa5, 0x2e, 0x31, 0x64, 0x52, 0x48, 0x06, 0x5c, 0x8d, 0xe9, 0x08, 0x9a, 0x0f, 
        0x21, 0x50, 0x70, 0x01, 0xda, 0xe2, 0xc6, 0x27, 0x95, 0xae, 0xc1, 0xeb, 0x5c, 0xed, 0x61, 0x1f, 
        0x7f, 0xc8, 0x36, 0x10, 0x29, 0xe7, 0x9f, 0xdd, 0xd6, 0x10, 0x1f, 0xae, 0x01, 0x2a, 0x76, 0x6f, 
        0xff, 0x06, 0x05, 0xeb, 0xea, 0x7f, 0xe1, 0xec, 0xef, 0x97, 0xea, 0x66, 0x22, 0x4e, 0xe9, 0xdb, 
        0xa1, 0x29, 0x0a, 0x44, 0x50, 0xbe, 0xb2, 0xd7, 0x23, 0x03, 0xdb, 0x03, 0x32, 0xa3, 0x60, 0x70, 
        0x43, 0x35, 0x29, 0xb0, 0x11, 0xbf, 0x4f, 0xd1, 0x63, 0x47, 0x1b, 0x9e, 0x46, 0x66, 0x3e, 0x10, 
        0x29, 0x2f, 0x55, 0x4e, 0x07, 0xb6, 0xc5, 0xdb, 0x67, 0xd9, 0xfc, 0xb5, 0x5b, 0x7d, 0x10, 0x0e, 
        0x0a, 0x54, 0xf2, 0x92, 0x61, 0x83, 0x9f, 0xf6, 0x01, 0xc3, 0xf0, 0xd3, 0x01, 0x99, 0xf4, 0xab, 
        0xff, 0x99, 0x04, 0x85, 0x22, 0x8b, 0xe3, 0x35, 0x1a, 0x58, 0xa7, 0xf1, 0xc9, 0xdf, 0x69, 0x13, 
        0xf9, 0x9b, 0x10, 0x0a, 0x4e, 0x3d, 0x4c, 0x10, 0x8d, 0x98, 0xb8, 0xbe, 0xd2, 0xc5, 0xf5, 0x89, 
        0xf6, 0x67, 0xbe, 0x33, 0x45, 0xbd, 0x51, 0x01, 0x21, 0x7b, 0x83, 0xd6, 0x10, 0x79, 0x57, 0x1e, 
        0x35, 0x8c, 0x5c, 0xf5, 0x5a, 0x5e, 0xa5, 0x95, 0xff, 0xd3, 0x73, 0x68, 0xef, 0xbe, 0x73, 0x6c, 
        0x73, 0x98, 0x1c, 0x0d, 0x7a, 0xae, 0x8e, 0x2e, 0x34, 0xce, 0x81, 0xff, 0x27, 0xda, 0x63, 0x63, 
        0x3b, 0xb9, 0x5b, 0xa2, 0xa3, 0xef, 0x76, 0xed, 0x5f, 0x8d, 0xef, 0x39, 0xa3, 0x43, 0x31, 0x62, 
        0xfb, 0x54, 0x92, 0x2f, 0x43, 0x0c, 0x45, 0xb7, 0xc5, 0x7f, 0xd3, 0x62, 0x93, 0x5b, 0x48, 0x7a, 
        0x3f, 0x1b, 0x8e, 0x8e, 0x07, 0x9d, 0xc9, 0x86, 0x48, 0xd5, 0x2a, 0x7b, 0x50, 0xb5, 0xf2, 0xc4, 
        0xf4, 0x34, 0x36, 0xe5, 0x61, 0xcb, 0x5f, 0x53, 0xa8, 0x66, 0x53, 0x55, 0x59, 0x6e, 0xbe, 0xea, 
        0xde, 0x28, 0x4b, 0x0b, 0xa2, 0xed, 0xeb, 0x6c, 0x48, 0x27, 0x56, 0xa4, 0xe1, 0xcf, 0x3a, 0x9a, 
        0xdc, 0x7c, 0xae, 0x7d, 0x18, 0x66, 0x93, 0x54, 0xb9, 0xc4, 0x2c, 0x5f, 0xf3, 0x0d, 0x4a, 0x33, 
        0xe4, 0x6e, 0x4d, 0x92, 0xe2, 0xe3, 0x06, 0x3c, 0x5f, 0x42, 0x29, 0x8e, 0x59, 0x96, 0xb0, 0xa5, 
        0xa5, 0xed, 0x03, 0x69, 0x22, 0xba, 0x12, 0x76, 0xcc, 0x7b, 0xea, 0xac, 0x95, 0x02, 0x47, 0xa1, 
        0xda, 0x01, 0x57, 0x79, 0xf4, 0xd8, 0xfb, 0xa9, 0xe2, 0x91, 0xc5, 0x70, 0xd5, 0x50, 0xe0, 0x45, 
        0x10, 0x0e, 0x18, 0x66, 0xa7, 0xa9, 0x29, 0x68, 0x90, 0xf8, 0x44, 0x60, 0x75, 0xd2, 0x66, 0xa1, 
        0x2d, 0xde, 0x10, 0xe1, 0xce, 0xbc, 0xd6, 0xcb, 0x7d, 0x67, 0xf5, 0xd3, 0x32, 0x2e, 0x10, 0x09, 
        0x2b, 0x0e, 0x02, 0x6c, 0xe8, 0xc9, 0x90, 0xbf, 0xbb, 0x96, 0x5c, 0x78, 0x60, 0x85, 0xf3, 0x52, 
        0xa1, 0xa2, 0x6c, 0xff, 0x88, 0x09, 0x06, 0x11, 0x13, 0xd7, 0x7e, 0x3d, 0x98, 0x82, 0x01, 0x9c, 
        0x49, 0x10, 0x8c, 0x30, 0xdd, 0xe8, 0x1f, 0x20, 0x37, 0x37, 0x30, 0x0d, 0x53, 0x0c, 0x92, 0x20, 
};

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
          // MOV EAX, EDI
          if (!push_2bytesxcode(xcode, 0x8b, 0xc7))
              return 0;
        break;
      case 0:
        // MOV EAX, xcode_rand(xcode)
        if (!push_bytexcode(xcode, 0xb8)
                || !push_dwordxcode(xcode, xcode_rand(xcode)))
            return 0;
          break;
      }
      return 1;
}

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
    case 2:
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
    case 0:
        // DEC EAX
        if (!push_bytexcode(xcode, 0x48))
            return 0;
        break;
    case 7:
        // NEG EAX
        if (!push_2bytesxcode(xcode, 0xf7, 0xd8))
            return 0;
        break;
    case 5:
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
    case 4:
        // INC EAX
        if (!push_bytexcode(xcode, 0x40))
            return 0;
        break;
    case 1:
        // XOR EAX, xcode_rand(xcode)
        if (!push_bytexcode(xcode, 0x35) 
                || !push_dwordxcode(xcode, xcode_rand(xcode)))
            return 0;
        break;
    }
    return 1;
}

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
    case 2:
        // ADD EAX, EBX
        if (!push_2bytesxcode(xcode, 0x01, 0xd8))
            return 0;
        break;
    case 3:
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
    case 1:
        // NEG EAX, ADD EAX, EBX
        if (!push_2bytesxcode(xcode, 0xf7, 0xd8) 
                || !push_2bytesxcode(xcode, 0x01, 0xd8))
            return 0;
        break;
    case 4:
        // IMUL EAX, EBX
        if (!push_3bytesxcode(xcode, 0x0f,  0xaf, 0xc3))
            return 0;
        break;
    case 5:
        // SUB EAX, EBX
        if (!push_2bytesxcode(xcode, 0x29, 0xd8))
            return 0;
        break;
    }
    // POP EBX
    return push_bytexcode(xcode, 0x5b);
}

struct cxdec_callback ojyo_cxdec_callback = {
    "ojyo",
    { 0x161, 0x5c9 },
    xcode_building_stage1
};
