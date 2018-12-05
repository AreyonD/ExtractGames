#include "cxdec.h"

static int xcode_building_stage0(struct cxdec_xcode_status *xcode, int stage);
static int xcode_building_stage1(struct cxdec_xcode_status *xcode, int stage);

static BYTE EncryptionControlBlock[4096] = {
        0x20, 0x45, 0x6E, 0x63, 0x72, 0x79, 0x70, 0x74, 0x69, 0x6F, 
0x6E, 0x20, 0x63, 0x6F, 0x6E, 0x74, 0x72, 0x6F, 0x6C, 0x20, 
0x62, 0x6C, 0x6F, 0x63, 0x6B, 0x20, 0x2D, 0x2D, 0x20, 0x53, 
0x74, 0x61, 0x74, 0x69, 0x63, 0x61, 0x6C, 0x6C, 0x79, 0x20, 
0x6F, 0x72, 0x20, 0x64, 0x79, 0x6E, 0x61, 0x6D, 0x69, 0x63, 
0x61, 0x6C, 0x6C, 0x79, 0x2C, 0x20, 0x64, 0x69, 0x72, 0x65, 
0x63, 0x74, 0x6C, 0x79, 0x20, 0x6F, 0x72, 0x20, 0x69, 0x6E, 
0x64, 0x69, 0x72, 0x65, 0x63, 0x74, 0x6C, 0x79, 0x2C, 0x20, 
0x75, 0x73, 0x69, 0x6E, 0x67, 0x20, 0x74, 0x68, 0x69, 0x73, 
0x20, 0x70, 0x72, 0x6F, 0x67, 0x72, 0x61, 0x6D, 0x20, 0x61, 
0x6E, 0x64, 0x2F, 0x6F, 0x72, 0x20, 0x62, 0x6C, 0x6F, 0x63, 
0x6B, 0x20, 0x66, 0x72, 0x6F, 0x6D, 0x20, 0x6F, 0x74, 0x68, 
0x65, 0x72, 0x20, 0x70, 0x72, 0x6F, 0x67, 0x72, 0x61, 0x6D, 
0x73, 0x20, 0x77, 0x69, 0x6C, 0x6C, 0x20, 0x62, 0x65, 0x20, 
0x69, 0x6C, 0x6C, 0x65, 0x67, 0x61, 0x6C, 0x20, 0x62, 0x79, 
0x20, 0x74, 0x68, 0x65, 0x20, 0x6C, 0x69, 0x63, 0x65, 0x6E, 
0x73, 0x65, 0x20, 0x61, 0x67, 0x72, 0x65, 0x65, 0x6D, 0x65, 
0x6E, 0x74, 0x2E, 0x20, 0x82, 0xB1, 0x82, 0xCC, 0x83, 0x76, 
0x83, 0x8D, 0x83, 0x4F, 0x83, 0x89, 0x83, 0x80, 0x82, 0xE2, 
0x83, 0x75, 0x83, 0x8D, 0x83, 0x62, 0x83, 0x4E, 0x82, 0xF0, 
0x81, 0x41, 0x90, 0xC3, 0x93, 0x49, 0x82, 0xC5, 0x82, 0xA0, 
0x82, 0xEA, 0x93, 0xAE, 0x93, 0x49, 0x82, 0xC5, 0x82, 0xA0, 
0x82, 0xEA, 0x81, 0x41, 0x92, 0xBC, 0x90, 0xDA, 0x93, 0x49, 
0x82, 0xC5, 0x82, 0xA0, 0x82, 0xEA, 0x8A, 0xD4, 0x90, 0xDA, 
0x93, 0x49, 0x82, 0xC5, 0x82, 0xA0, 0x82, 0xEA, 0x81, 0x41, 
0x91, 0xBC, 0x82, 0xCC, 0x83, 0x76, 0x83, 0x8D, 0x83, 0x4F, 
0x83, 0x89, 0x83, 0x80, 0x82, 0xA9, 0x82, 0xE7, 0x97, 0x70, 
0x82, 0xA2, 0x82, 0xE9, 0x82, 0xB1, 0x82, 0xC6, 0x82, 0xCD, 
0x83, 0x89, 0x83, 0x43, 0x83, 0x5A, 0x83, 0x93, 0x83, 0x58, 
0x82, 0xC9, 0x82, 0xE6, 0x82, 0xE8, 0x8B, 0xD6, 0x82, 0xB6, 
0x82, 0xE7, 0x82, 0xEA, 0x82, 0xC4, 0x82, 0xA2, 0x82, 0xDC, 
0x82, 0xB7, 0x81, 0x42, 0x0A, 0x28, 0x43, 0x29, 0x6F, 0x6E, 
0x6F, 0x6D, 0x61, 0x74, 0x6F, 0x70, 0x65, 0x2A, 0x20, 0x61, 
0x6C, 0x6C, 0x20, 0x72, 0x69, 0x67, 0x68, 0x74, 0x73, 0x20, 
0x72, 0x65, 0x73, 0x65, 0x72, 0x76, 0x65, 0x64, 0x2E, 0x0A, 
0x0A, 0x89, 0xB4, 0x82, 0xC9, 0x82, 0xCD, 0x81, 0x41, 0x94, 
0xDE, 0x8F, 0x97, 0x82, 0xAA, 0x82, 0xA2, 0x82, 0xC8, 0x82, 
0xA2, 0x0A, 0x82, 0xBE, 0x82, 0xAF, 0x82, 0xC7, 0x81, 0x41, 
0x8B, 0x43, 0x82, 0xC9, 0x82, 0xC8, 0x82, 0xE9, 0x96, 0xBA, 
0x82, 0xCD, 0x82, 0xA2, 0x82, 0xE9, 0x0A, 0x90, 0x6C, 0x90, 
0xB6, 0x82, 0xC5, 0x88, 0xEA, 0x93, 0x78, 0x82, 0xAE, 0x82, 
0xE7, 0x82, 0xA2, 0x82, 0xCD, 0x96, 0x7B, 0x8B, 0x43, 0x82, 
0xCC, 0x97, 0xF6, 0x82, 0xF0, 0x82, 0xB5, 0x82, 0xBD, 0x82, 
0xA2, 0x81, 0x49, 0x0A, 0x90, 0x6C, 0x90, 0xB6, 0x82, 0xC5, 
0x88, 0xEA, 0x93, 0x78, 0x82, 0xAE, 0x82, 0xE7, 0x82, 0xA2, 
0x82, 0xCD, 0x96, 0x7B, 0x8B, 0x43, 0x82, 0xCC, 0x8D, 0x90, 
0x94, 0x92, 0x82, 0xF0, 0x82, 0xB5, 0x82, 0xBD, 0x82, 0xA2, 
0x81, 0x49, 0x0A, 0x0A, 0x82, 0xBB, 0x82, 0xB5, 0x82, 0xC4, 
0x81, 0x41, 0x89, 0xB4, 0x82, 0xCD, 0x8D, 0x73, 0x93, 0xAE, 
0x82, 0xB7, 0x82, 0xE9, 0x0A, 0x0A, 0x96, 0xB2, 0x82, 0xCC, 
0x82, 0xA2, 0x82, 0xBF, 0x82, 0xE1, 0x82, 0xA2, 0x82, 0xBF, 
0x82, 0xE1, 0x90, 0xB6, 0x8A, 0x88, 0x82, 0xCC, 0x82, 0xBD, 
0x82, 0xDF, 0x82, 0xC9, 0x81, 0x49, 0x81, 0x49, 0x0A, 0x0A, 
0x0A, 0x92, 0x4E, 0x82, 0xC6, 0x82, 0xE0, 0x95, 0x74, 0x82, 
0xAB, 0x8D, 0x87, 0x82, 0xC1, 0x82, 0xBD, 0x82, 0xB1, 0x82, 
0xC6, 0x82, 0xCC, 0x82, 0xC8, 0x82, 0xA2, 0x96, 0x7D, 0x90, 
0x6C, 0x8C, 0x6E, 0x8E, 0xE5, 0x90, 0x6C, 0x8C, 0xF6, 0x82, 
0xAA, 0x0A, 0x88, 0xEA, 0x94, 0x4F, 0x94, 0xAD, 0x8B, 0x4E, 
0x82, 0xB5, 0x82, 0xC4, 0x88, 0xD3, 0x92, 0x86, 0x82, 0xCC, 
0x94, 0xDE, 0x8F, 0x97, 0x82, 0xC9, 0x81, 0x77, 0x8D, 0x90, 
0x94, 0x92, 0x81, 0x78, 0x0A, 0x0A, 0x82, 0xBB, 0x82, 0xB5, 
0x82, 0xC4, 0x81, 0x41, 0x94, 0x4F, 0x8A, 0xE8, 0x82, 0xA9, 
0x82, 0xC8, 0x82, 0xC1, 0x82, 0xC4, 0x94, 0xDE, 0x8E, 0x81, 
0x94, 0xDE, 0x8F, 0x97, 0x82, 0xCC, 0x97, 0xA7, 0x8F, 0xEA, 
0x82, 0xC9, 0x81, 0x49, 0x81, 0x48, 0x0A, 0x0A, 0x0A, 0x95, 
0xA8, 0x8C, 0xEA, 0x82, 0xC5, 0x82, 0xC2, 0x82, 0xC3, 0x82, 
0xE7, 0x82, 0xEA, 0x82, 0xC4, 0x82, 0xA2, 0x82, 0xAD, 0x82, 
0xCC, 0x82, 0xCD, 0x0A, 0x0A, 0x90, 0xA2, 0x82, 0xCC, 0x92, 
0x86, 0x82, 0xCC, 0x83, 0x4A, 0x83, 0x62, 0x83, 0x76, 0x83, 
0x8B, 0x82, 0xAA, 0x8C, 0x6F, 0x8C, 0xB1, 0x82, 0xB5, 0x82, 
0xC4, 0x82, 0xA2, 0x82, 0xAD, 0x97, 0xF6, 0x88, 0xA4, 0x82, 
0xCC, 0x82, 0xB7, 0x82, 0xD7, 0x82, 0xC4, 0x81, 0x49, 0x0A, 
0x0A, 0x82, 0xCD, 0x82, 0xB6, 0x82, 0xDF, 0x82, 0xC4, 0x82, 
0xCC, 0x8D, 0x90, 0x94, 0x92, 0x0A, 0x82, 0xCD, 0x82, 0xB6, 
0x82, 0xDF, 0x82, 0xC4, 0x82, 0xCC, 0x94, 0xDE, 0x8E, 0x81, 
0x94, 0xDE, 0x8F, 0x97, 0x0A, 0x82, 0xCD, 0x82, 0xB6, 0x82, 
0xDF, 0x82, 0xC4, 0x82, 0xCC, 0x88, 0xEA, 0x8F, 0x8F, 0x82, 
0xCC, 0x89, 0xBA, 0x8D, 0x5A, 0x0A, 0x82, 0xCD, 0x82, 0xB6, 
0x82, 0xDF, 0x82, 0xC4, 0x82, 0xCC, 0x8E, 0xE8, 0x82, 0xC2, 
0x82, 0xC8, 0x82, 0xAC, 0x0A, 0x82, 0xCD, 0x82, 0xB6, 0x82, 
0xDF, 0x82, 0xC4, 0x82, 0xCC, 0x83, 0x81, 0x81, 0x5B, 0x83, 
0x8B, 0x0A, 0x82, 0xCD, 0x82, 0xB6, 0x82, 0xDF, 0x82, 0xC4, 
0x82, 0xCC, 0x88, 0xEA, 0x8F, 0x8F, 0x82, 0xCC, 0x93, 0x6F, 
0x8D, 0x5A, 0x0A, 0x82, 0xCD, 0x82, 0xB6, 0x82, 0xDF, 0x82, 
0xC4, 0x82, 0xCC, 0x88, 0xEA, 0x8F, 0x8F, 0x82, 0xCC, 0x95, 
0xFA, 0x89, 0xDB, 0x8C, 0xE3, 0x0A, 0x82, 0xCD, 0x82, 0xB6, 
0x82, 0xDF, 0x82, 0xC4, 0x82, 0xCC, 0x83, 0x66, 0x81, 0x5B, 
0x83, 0x67, 0x0A, 0x82, 0xCD, 0x82, 0xB6, 0x82, 0xDF, 0x82, 
0xC4, 0x82, 0xCC, 0x83, 0x76, 0x83, 0x8A, 0x83, 0x93, 0x83, 
0x67, 0x83, 0x56, 0x81, 0x5B, 0x83, 0x8B, 0x0A, 0x82, 0xCD, 
0x82, 0xB6, 0x82, 0xDF, 0x82, 0xC4, 0x82, 0xCC, 0x83, 0x4A, 
0x83, 0x89, 0x83, 0x49, 0x83, 0x50, 0x0A, 0x82, 0xCD, 0x82, 
0xB6, 0x82, 0xDF, 0x82, 0xC4, 0x82, 0xCC, 0x83, 0x4C, 0x83, 
0x58, 0x0A, 0x82, 0xCD, 0x82, 0xB6, 0x82, 0xDF, 0x82, 0xC4, 
0x82, 0xCC, 0x81, 0x45, 0x81, 0x45, 0x81, 0x45, 0x0A, 0x83, 
0x47, 0x83, 0x67, 0x83, 0x5A, 0x83, 0x67, 0x83, 0x89, 0x0A, 
0x83, 0x47, 0x83, 0x67, 0x83, 0x5A, 0x83, 0x67, 0x83, 0x89, 
0x0A, 0x83, 0x47, 0x83, 0x67, 0x83, 0x5A, 0x83, 0x67, 0x83, 
0x89, 0x0A, 0x83, 0x47, 0x83, 0x67, 0x83, 0x5A, 0x83, 0x67, 
0x83, 0x89, 0x0A, 0x0A, 0x82, 0xBB, 0x82, 0xF1, 0x82, 0xC8, 
0x0A, 0x97, 0xF6, 0x90, 0x6C, 0x82, 0xC6, 0x82, 0xC5, 0x82, 
0xAB, 0x82, 0xE9, 0x81, 0x77, 0x82, 0xCD, 0x82, 0xB6, 0x82, 
0xDF, 0x82, 0xC4, 0x81, 0x78, 0x82, 0xF0, 0x0A, 0x82, 0xBD, 
0x82, 0xAD, 0x82, 0xB3, 0x82, 0xF1, 0x8C, 0x6F, 0x8C, 0xB1, 
0x82, 0xB5, 0x82, 0xC4, 0x0A, 0x94, 0xDE, 0x8F, 0x97, 0x82, 
0xC6, 0x82, 0xCC, 0x8A, 0x7C, 0x91, 0xD6, 0x82, 0xA6, 0x82, 
0xCC, 0x82, 0xC8, 0x82, 0xA2, 0x8E, 0x9E, 0x8A, 0xD4, 0x82, 
0xF0, 0x89, 0xDF, 0x82, 0xB2, 0x82, 0xB5, 0x0A, 0x82, 0xBE, 
0x82, 0xF1, 0x82, 0xBE, 0x82, 0xF1, 0x82, 0xC6, 0x83, 0x47, 
0x83, 0x58, 0x83, 0x4A, 0x83, 0x8C, 0x81, 0x5B, 0x83, 0x67, 
0x82, 0xB5, 0x82, 0xC4, 0x82, 0xA2, 0x82, 0xAD, 0x93, 0xF1, 
0x90, 0x6C, 0x82, 0xCC, 0x8A, 0xD6, 0x8C, 0x57, 0x0A, 0x82, 
0xE2, 0x82, 0xAA, 0x82, 0xC4, 0x81, 0x41, 0x93, 0xF1, 0x90, 
0x6C, 0x82, 0xCD, 0x0A, 0x81, 0x77, 0x82, 0xC7, 0x82, 0xB1, 
0x82, 0xC9, 0x8F, 0x6F, 0x82, 0xB5, 0x82, 0xC4, 0x82, 0xE0, 
0x92, 0x70, 0x82, 0xB8, 0x82, 0xA9, 0x82, 0xB5, 0x82, 0xA2, 
0x83, 0x6F, 0x83, 0x4A, 0x83, 0x62, 0x83, 0x76, 0x83, 0x8B, 
0x81, 0x78, 0x82, 0xC9, 0x90, 0xAC, 0x92, 0xB7, 0x82, 0xB5, 
0x82, 0xC4, 0x82, 0xA2, 0x82, 0xAD, 0x81, 0x49, 0x0A, 0xF0, 
0x25, 0x81, 0x2D, 0xE1, 0xFA, 0xA6, 0x41, 0x3B, 0xB6, 0xA3, 
0xED, 0x2D, 0x62, 0x69, 0x5F, 0x97, 0x05, 0x9F, 0x04, 0xB0, 
0x0E, 0x22, 0xE7, 0xA5, 0xB9, 0x91, 0x61, 0x99, 0x5A, 0x4F, 
0x5C, 0x68, 0xDA, 0x28, 0x8D, 0xAC, 0x07, 0xBD, 0x6B, 0xF1, 
0xA4, 0xD2, 0xC0, 0x34, 0xB4, 0xDB, 0xE6, 0x6B, 0xFD, 0xB2, 
0x8A, 0x3E, 0x79, 0xBF, 0x22, 0x77, 0xAC, 0x59, 0xFA, 0x48, 
0xE4, 0xD1, 0x2B, 0x8A, 0x9A, 0x1D, 0x34, 0xE7, 0xDD, 0xCF, 
0x89, 0x13, 0x3B, 0x85, 0x81, 0x0B, 0xFC, 0x3E, 0xCB, 0x98, 
0x01, 0x5F, 0x35, 0xB6, 0xC4, 0xD5, 0x56, 0xFC, 0xD5, 0xEB, 
0x80, 0xAF, 0x0D, 0x04, 0xF2, 0xF0, 0x93, 0x6C, 0x3C, 0x22, 
0x32, 0xF0, 0x04, 0x3D, 0x7D, 0x5E, 0xE0, 0x6E, 0x76, 0x6A, 
0x75, 0xA5, 0x92, 0x1F, 0x51, 0x51, 0x54, 0x52, 0xB9, 0x49, 
0xDB, 0x53, 0x62, 0xF2, 0x25, 0xAA, 0xEF, 0x5C, 0x84, 0x48, 
0xD7, 0x70, 0x3D, 0x06, 0x46, 0x4B, 0x55, 0x81, 0x3A, 0x1C, 
0xF4, 0x09, 0xE3, 0x5B, 0x71, 0xE8, 0xBE, 0x4B, 0x65, 0x48, 
0x1D, 0x34, 0xDB, 0xDF, 0x51, 0x4B, 0x4E, 0x5A, 0x9E, 0xD2, 
0x32, 0xE3, 0xF6, 0x67, 0xCA, 0x7D, 0x8E, 0x5E, 0xF2, 0x37, 
0xB3, 0xE9, 0x2B, 0x1C, 0xD4, 0x47, 0xA3, 0x5A, 0x03, 0xF5, 
0x84, 0xCB, 0x90, 0x08, 0xAF, 0x74, 0x44, 0x48, 0x03, 0x7E, 
0x93, 0x1A, 0xA4, 0x36, 0xD1, 0x80, 0xBB, 0x02, 0x01, 0x39, 
0xC0, 0x5F, 0x15, 0x7B, 0xE6, 0xF7, 0x1E, 0x9D, 0x93, 0x28, 
0x39, 0xA3, 0xD3, 0x33, 0x6A, 0xA2, 0x9E, 0x87, 0x43, 0xBC, 
0x7B, 0x52, 0x34, 0xCF, 0xB6, 0xCD, 0xB2, 0x1B, 0xF3, 0xDA, 
0x97, 0xC8, 0x98, 0x59, 0xBB, 0x10, 0xF1, 0x41, 0x69, 0x32, 
0xEF, 0x88, 0xBE, 0xAE, 0x0A, 0xEF, 0x44, 0xC9, 0xFE, 0x25, 
0xB0, 0x39, 0x48, 0x9A, 0xEC, 0xA2, 0xE7, 0x48, 0x88, 0x1C, 
0x6B, 0x58, 0x4C, 0xB8, 0x13, 0x5B, 0x15, 0xB5, 0x5E, 0xF6, 
0xB0, 0xA0, 0x27, 0x84, 0x31, 0xD1, 0x63, 0xFC, 0x9A, 0x41, 
0xA9, 0x6D, 0xEA, 0xFB, 0xCE, 0xD9, 0xF8, 0x17, 0x0C, 0x5C, 
0x8A, 0x67, 0x30, 0x6B, 0x1E, 0xF7, 0x8A, 0x9E, 0xE8, 0x0F, 
0x91, 0x17, 0x88, 0xC6, 0x13, 0xAB, 0xC8, 0xAE, 0x2B, 0x73, 
0xD0, 0xA5, 0x0D, 0x91, 0xA8, 0x90, 0x82, 0x7B, 0xAE, 0xF7, 
0x3D, 0x51, 0xE9, 0xF7, 0xD2, 0x8F, 0x12, 0xCD, 0x42, 0xEE, 
0x7B, 0xC6, 0x95, 0xE6, 0x41, 0xD5, 0xCD, 0x1D, 0x8D, 0xEA, 
0x47, 0x79, 0x3C, 0xE7, 0x80, 0xDA, 0xB4, 0x07, 0xF1, 0xBE, 
0x22, 0x4C, 0x17, 0xB4, 0x46, 0x4D, 0xF4, 0x58, 0xFE, 0x43, 
0x3E, 0x3E, 0x6B, 0x04, 0x78, 0xBD, 0xC6, 0x13, 0x31, 0x23, 
0x8C, 0xAC, 0x1B, 0xCE, 0xB5, 0x77, 0x97, 0xD3, 0x6C, 0x08, 
0x2A, 0x21, 0x80, 0xC5, 0x47, 0x94, 0x3D, 0x68, 0xD1, 0x9D, 
0x9F, 0xA4, 0xC6, 0x03, 0xE8, 0xD9, 0xC8, 0x04, 0xD4, 0x9B, 
0x03, 0x86, 0xA7, 0xB2, 0xA2, 0xF2, 0x7A, 0xDE, 0x2D, 0x8B, 
0xD3, 0x8B, 0x51, 0x3E, 0x46, 0xEB, 0x62, 0xA5, 0xB1, 0xB0, 
0xA3, 0xB8, 0x85, 0x61, 0x89, 0xF6, 0x67, 0xB9, 0xBE, 0x3F, 
0xF9, 0x9A, 0x17, 0x55, 0xA2, 0x0B, 0xBB, 0x68, 0x9D, 0xF4, 
0x81, 0x65, 0x2F, 0x9E, 0x1D, 0xEC, 0x45, 0xC1, 0x42, 0xB2, 
0x2A, 0xEF, 0xBB, 0xD8, 0x43, 0x11, 0x47, 0xE5, 0x1F, 0x1D, 
0x13, 0x7E, 0x5E, 0x13, 0x49, 0x31, 0x18, 0xDD, 0x58, 0x0B, 
0xB4, 0xAC, 0x16, 0x98, 0xFA, 0xD4, 0xC9, 0x58, 0xE8, 0x66, 
0x80, 0x03, 0x43, 0x76, 0xCE, 0x00, 0x71, 0x8D, 0x45, 0xB0, 
0xDC, 0xAE, 0x82, 0x6C, 0xE8, 0xCC, 0x35, 0x52, 0xA4, 0x1F, 
0x6F, 0x86, 0x26, 0xF6, 0xB2, 0xB0, 0xD1, 0x40, 0x24, 0x16, 
0x56, 0x48, 0x1E, 0x3D, 0xDC, 0x14, 0x25, 0x07, 0xF9, 0x70, 
0xDB, 0x4C, 0x8E, 0xEB, 0x6F, 0x74, 0x95, 0x4D, 0xCD, 0x80, 
0x39, 0xE0, 0x3C, 0x5A, 0xC4, 0x9A, 0x02, 0x0B, 0xF8, 0x8D, 
0x14, 0x7B, 0xCE, 0x6D, 0xE0, 0x01, 0x66, 0x32, 0x0B, 0x96, 
0xAD, 0x2D, 0xF8, 0xC4, 0x3B, 0x7B, 0x8E, 0x84, 0xB6, 0xF7, 
0xA8, 0x9B, 0xDB, 0x2E, 0xFC, 0xDD, 0xAE, 0x84, 0xF4, 0xDF, 
0x65, 0x38, 0xCC, 0x60, 0x64, 0x5B, 0x4B, 0x88, 0xAA, 0x0A, 
0x91, 0x2E, 0x2F, 0xC7, 0x0A, 0xB4, 0xA9, 0x43, 0xD9, 0xE2, 
0x9F, 0xBA, 0xAF, 0xC6, 0xD4, 0x9F, 0x02, 0x36, 0x8B, 0x62, 
0x39, 0xE1, 0xD0, 0x53, 0x5C, 0x74, 0xFE, 0xF0, 0x4E, 0xAF, 
0xF3, 0xEA, 0xCC, 0x21, 0x28, 0x99, 0x99, 0x7F, 0xE6, 0xBE, 
0xCD, 0xA4, 0xA9, 0x5C, 0x11, 0xA5, 0x31, 0x5B, 0x03, 0x64, 
0x83, 0x27, 0x6A, 0x5C, 0x99, 0xC3, 0x10, 0x38, 0x17, 0x2A, 
0x67, 0x55, 0xE0, 0x25, 0xE3, 0x15, 0xC3, 0xC0, 0xBB, 0x88, 
0x11, 0xCD, 0x7B, 0xB7, 0x80, 0x38, 0x4F, 0x04, 0x12, 0x5E, 
0x4C, 0x67, 0xC5, 0x9B, 0x31, 0x41, 0xB3, 0x82, 0x0E, 0x01, 
0xED, 0xD0, 0x9C, 0xA0, 0xA4, 0x91, 0x64, 0xB7, 0x7F, 0x9C, 
0xDF, 0x59, 0x1F, 0xD3, 0xD9, 0x5A, 0xB6, 0xFC, 0x40, 0x41, 
0x3E, 0xBA, 0x29, 0x83, 0x51, 0x18, 0x3E, 0x82, 0x21, 0xDD, 
0xCC, 0xFB, 0xD3, 0xE7, 0x33, 0xC7, 0x85, 0xE8, 0xEE, 0x74, 
0xF2, 0xB1, 0x45, 0x11, 0x06, 0x38, 0x71, 0x21, 0x2E, 0x1A, 
0x7C, 0xBD, 0xBE, 0x51, 0xFD, 0x91, 0x0E, 0x4D, 0x04, 0xD7, 
0x41, 0xB6, 0xD4, 0x21, 0x70, 0x4D, 0xC8, 0x17, 0x20, 0x4B, 
0xA7, 0xCF, 0xF1, 0xDB, 0xF3, 0x4E, 0x15, 0x77, 0x4F, 0x74, 
0xD2, 0xC8, 0xF2, 0x31, 0x1C, 0x1F, 0xC0, 0x7E, 0xB5, 0xAE, 
0xC7, 0x0B, 0xEE, 0x74, 0x49, 0x47, 0xEE, 0x70, 0x9B, 0x70, 
0x14, 0x5A, 0xC5, 0xDE, 0x68, 0xF4, 0x40, 0x08, 0xF9, 0x91, 
0xC7, 0xC0, 0x8F, 0x5E, 0xE5, 0xA3, 0xCD, 0x1E, 0x6A, 0x4B, 
0x85, 0xF2, 0x75, 0xAF, 0x5C, 0x7D, 0xCB, 0xC4, 0xCC, 0x96, 
0xC2, 0x45, 0xD3, 0x6E, 0x97, 0x4D, 0x67, 0x70, 0xDB, 0xB8, 
0x3A, 0x07, 0xFA, 0xB2, 0xA3, 0x5D, 0xD9, 0x76, 0x45, 0x4F, 
0x8B, 0x8C, 0xBE, 0xF1, 0x4C, 0x52, 0xD0, 0xD2, 0x44, 0xAE, 
0x9D, 0xC7, 0x12, 0x20, 0xE5, 0x2E, 0x6C, 0xDA, 0x93, 0x91, 
0x33, 0x6A, 0x5D, 0xC1, 0xB5, 0x5F, 0x35, 0x86, 0xA8, 0x0A, 
0x3C, 0x17, 0xC5, 0xEB, 0x76, 0xAE, 0x4F, 0xAF, 0xD6, 0xC2, 
0xED, 0x35, 0xB7, 0x4E, 0x69, 0xCE, 0x95, 0x6B, 0x28, 0x5A, 
0xD2, 0x32, 0x8E, 0x50, 0x3F, 0x0C, 0x46, 0xB0, 0x3D, 0xDF, 
0x02, 0x85, 0x10, 0xEF, 0x3F, 0xD4, 0xD8, 0xEB, 0x05, 0x5F, 
0x89, 0x33, 0xA7, 0x9A, 0x05, 0x1B, 0x35, 0x9D, 0xC1, 0xB8, 
0x95, 0xE1, 0x20, 0xB0, 0xD4, 0x1F, 0xD1, 0xB7, 0x7B, 0x4F, 
0x39, 0x41, 0x57, 0xF0, 0x32, 0x91, 0x31, 0x07, 0xCF, 0xAE, 
0x87, 0xC4, 0xDF, 0x60, 0xD6, 0xB4, 0x46, 0xC2, 0x4C, 0x82, 
0x1F, 0x39, 0x0E, 0x8B, 0xCD, 0x3B, 0xC3, 0xD1, 0xC7, 0x15, 
0xCB, 0xFE, 0xA8, 0x4D, 0x6E, 0xFD, 0xD8, 0x91, 0xED, 0x2A, 
0x5F, 0xD3, 0x57, 0xBF, 0x4C, 0x95, 0xDF, 0x6B, 0x8B, 0x9D, 
0x45, 0x07, 0x63, 0xFB, 0xA5, 0x46, 0x2D, 0x59, 0x5C, 0xF3, 
0x99, 0x8E, 0xB2, 0x3F, 0x5E, 0xE1, 0x2F, 0xDC, 0x49, 0xC2, 
0x12, 0xA6, 0xAC, 0x79, 0x36, 0x08, 0x19, 0xA5, 0xD0, 0xBF, 
0xFE, 0xC5, 0xFA, 0x36, 0x1A, 0x8C, 0xEC, 0xA4, 0x98, 0x92, 
0xA3, 0xA6, 0xD3, 0xDE, 0xA8, 0x69, 0x46, 0x05, 0x5A, 0x03, 
0xF5, 0x0B, 0x1A, 0xE7, 0xF2, 0x50, 0x2B, 0x29, 0xA7, 0x65, 
0x86, 0xF0, 0x06, 0xAC, 0xAE, 0xF0, 0xB2, 0xFE, 0x1D, 0x50, 
0xB7, 0x54, 0x62, 0x9F, 0x24, 0x53, 0xD3, 0xBA, 0x8B, 0x64, 
0x70, 0xAF, 0x0C, 0x3A, 0x3C, 0xAE, 0x2C, 0xF4, 0xA2, 0x04, 
0xE5, 0xBD, 0xA7, 0x45, 0xA4, 0x86, 0xE5, 0xE9, 0xF0, 0xF8, 
0x44, 0x21, 0x9C, 0xA0, 0x34, 0x28, 0xD2, 0x12, 0xA7, 0xB0, 
0x6E, 0xEB, 0x77, 0x9A, 0xF2, 0x21, 0x08, 0x90, 0xED, 0x7E, 
0x49, 0x4A, 0x1D, 0x6C, 0x58, 0x25, 0xA9, 0xCB, 0xE3, 0x2B, 
0x19, 0x73, 0x81, 0x86, 0xDC, 0x6B, 0xDB, 0x1D, 0xE4, 0xD1, 
0xDD, 0xD0, 0x04, 0xCB, 0x08, 0xA8, 0x31, 0x92, 0x61, 0x26, 
0x9B, 0x8C, 0x92, 0x84, 0x15, 0x44, 0xD6, 0xE1, 0x3D, 0x95, 
0x58, 0xC1, 0x7A, 0x00, 0x0A, 0x93, 0xCA, 0xED, 0x6D, 0xA2, 
0x28, 0xCE, 0xB3, 0xF1, 0xB5, 0x8C, 0x54, 0x20, 0x77, 0x46, 
0x3B, 0xCC, 0x9F, 0x51, 0xD6, 0xBC, 0x6E, 0x69, 0xF0, 0x8A, 
0x9B, 0xA6, 0x81, 0x7B, 0xE1, 0xB0, 0x16, 0x2D, 0x67, 0x04, 
0x42, 0x59, 0x7B, 0xC7, 0x79, 0xC0, 0xFE, 0x78, 0x0A, 0x8D, 
0x73, 0x8C, 0xA3, 0xC2, 0x68, 0xD6, 0x7F, 0xC2, 0xDA, 0x96, 
0x48, 0x36, 0xBF, 0x83, 0xA5, 0x3D, 0x41, 0x9A, 0xA2, 0x42, 
0x6F, 0xB1, 0x06, 0x8D, 0x94, 0x8F, 0xE6, 0xBB, 0x20, 0x1C, 
0x70, 0xFB, 0xDE, 0xF0, 0xA7, 0xA0, 0x35, 0x34, 0xFC, 0xF8, 
0xCE, 0xD6, 0x27, 0xFE, 0x3F, 0xF2, 0x49, 0x01, 0x32, 0xB7, 
0x7D, 0x7E, 0xA7, 0x39, 0x3B, 0x1C, 0xAA, 0x4E, 0xC3, 0xB6, 
0xEF, 0x28, 0x61, 0xCF, 0xED, 0x5B, 0xE8, 0x3B, 0xAD, 0xDC, 
0xFF, 0x57, 0x54, 0x4E, 0x5A, 0xC0, 0x80, 0x8F, 0xCA, 0xD1, 
0x4F, 0x19, 0x9E, 0x73, 0xB6, 0xA4, 0x26, 0x96, 0xC8, 0x40, 
0xDD, 0xDA, 0x94, 0xF6, 0x0A, 0xBC, 0xB0, 0x73, 0x9E, 0xD2, 
0x4A, 0xCE, 0x8F, 0xE9, 0x4C, 0xA3, 0xDD, 0xBD, 0x6D, 0x0B, 
0xA8, 0xCF, 0x37, 0x0F, 0xFC, 0x0C, 0x19, 0x51, 0x5F, 0x20, 
0x60, 0xF4, 0xC7, 0x53, 0x8B, 0x1F, 0xEF, 0xD0, 0x95, 0x75, 
0x82, 0xF5, 0x0A, 0xE4, 0xD8, 0x4E, 0x96, 0xFC, 0x45, 0x5C, 
0x43, 0x6B, 0xB7, 0xAE, 0x0D, 0xC2, 0x12, 0x20, 0xCE, 0xAF, 
0xD4, 0x85, 0x36, 0x0D, 0xE3, 0x8F, 0x67, 0xFF, 0xD3, 0xE5, 
0x65, 0x82, 0x63, 0xAA, 0x6E, 0x39, 0xD9, 0xB8, 0xE9, 0x98, 
0x09, 0x87, 0x58, 0xF5, 0x8F, 0x38, 0x48, 0xEA, 0x83, 0x5F, 
0x29, 0x84, 0x35, 0x5F, 0x11, 0x04, 0xBD, 0xE3, 0xFA, 0x4A, 
0xD1, 0x31, 0xE6, 0xBF, 0xA4, 0xC8, 0xCB, 0x29, 0xD4, 0xF0, 
0x5E, 0x08, 0x35, 0x6A, 0x40, 0xB6, 0xCC, 0x47, 0x43, 0x99, 
0xF2, 0x5C, 0x63, 0xA9, 0x87, 0xA4, 0x8E, 0x64, 0x63, 0xF7, 
0xAE, 0xC4, 0xA6, 0xE9, 0x37, 0x53, 0xAB, 0x8D, 0x5B, 0x6B, 
0xDF, 0x82, 0xE7, 0xF3, 0xD8, 0xC9, 0xA7, 0xDD, 0x9C, 0xFE, 
0xAF, 0x1E, 0x87, 0xED, 0x37, 0x4B, 0xA5, 0x45, 0xDD, 0xBC, 
0x64, 0x67, 0x3F, 0x0A, 0xE7, 0x08, 0x89, 0x7C, 0x72, 0x79, 
0x2D, 0x42, 0x68, 0xFA, 0xF2, 0x51, 0x96, 0xF8, 0x0A, 0xC0, 
0xC6, 0x7F, 0x3C, 0x61, 0x8F, 0xEE, 0xD4, 0xDC, 0x28, 0xFE, 
0x72, 0xA0, 0xF8, 0x20, 0x3D, 0x47, 0xE7, 0x1A, 0xD1, 0x48, 
0x88, 0x12, 0xD2, 0xCD, 0x9B, 0xBF, 0xC7, 0x31, 0xA6, 0xE2, 
0x3D, 0xF6, 0x90, 0x44, 0x74, 0xAD, 0x2A, 0xDB, 0x4D, 0x90, 
0x04, 0x9B, 0x86, 0xB8, 0x57, 0xF9, 0x4F, 0x80, 0x82, 0x44, 
0x17, 0x5F, 0x7F, 0xAF, 0xE0, 0x37, 0xFC, 0x05, 0xC0, 0xA3, 
0x43, 0x35, 0xA7, 0xDA, 0x56, 0x18, 0xF0, 0xD1, 0xBE, 0x9B, 
0x9C, 0xBF, 0xCB, 0x7E, 0x64, 0xFD, 0xBD, 0x09, 0xDB, 0x79, 
0x54, 0xBA, 0x3B, 0x69, 0xFD, 0xFE, 0xEF, 0x87, 0xE2, 0x5A, 
0x7B, 0xBE, 0x3C, 0x70, 0x72, 0x1E, 0x57, 0x1C, 0x12, 0x9D, 
0xB2, 0x52, 0x7B, 0x98, 0x6A, 0x9B, 0x7C, 0x82, 0x8F, 0x3A, 
0x60, 0x3D, 0x4F, 0xC8, 0x1A, 0x90, 0x8C, 0x00, 0xEE, 0x02, 
0x68, 0x1A, 0x5A, 0xF9, 0x45, 0xFC, 0x5D, 0xB5, 0x6A, 0xC2, 
0xFA, 0xFB, 0x02, 0xDC, 0x6B, 0x4A, 0x8B, 0xFD, 0x53, 0xDA, 
0x7F, 0x5F, 0x2D, 0xA9, 0x8A, 0xCB, 0xED, 0x65, 0xCD, 0x89, 
0x2E, 0xB2, 0xFC, 0xFC, 0xD3, 0xE5, 0xA6, 0x82, 0x7A, 0xD7, 
0xE4, 0xD6, 0xE1, 0x54, 0x8F, 0x57, 0x2D, 0x9D, 0xE0, 0xF1, 
0xB2, 0x1A, 0x88, 0x09, 0x98, 0xAF, 0xEF, 0x36, 0xFB, 0xDE, 
0x60, 0x42, 0x55, 0xC8, 0x62, 0xB6, 0xB2, 0xD0, 0xAA, 0xAF, 
0x2F, 0xE0, 0xB2, 0x23, 0xA1, 0x64, 0xCE, 0xC5, 0x1B, 0x58, 
0xAC, 0x47, 0xA3, 0x3E, 0x28, 0x51, 0x79, 0x97, 0xAB, 0xC0, 
0xA1, 0x74, 0x2D, 0xA7, 0x0D, 0x47, 0xA1, 0xC2, 0xC3, 0x63, 
0x62, 0x05, 0xF9, 0xD0, 0x85, 0xB8, 0xAA, 0x12, 0x37, 0xCE, 
0x30, 0x17, 0x9B, 0xF4, 0x47, 0x28, 0xEE, 0x23, 0x7B, 0x38, 
0xBF, 0x21, 0x12, 0x22, 0x6C, 0xBB, 0xD2, 0xB3, 0x92, 0xFF, 
0x77, 0x85, 0xDE, 0xDA, 0x95, 0xEB, 0xCE, 0x89, 0x30, 0x81, 
0x55, 0x6C, 0x7C, 0x9D, 0x98, 0x84, 0xE3, 0xBA, 0xD1, 0xBE, 
0x31, 0x43, 0x84, 0xB9, 0x02, 0xFA, 0x7E, 0xC1, 0x39, 0xAC, 
0xE7, 0xF2, 0x68, 0x34, 0xC8, 0x58, 0x56, 0xB9, 0x68, 0x31, 
0xA8, 0x9E, 0x4B, 0xA0, 0x52, 0xEF, 0xD3, 0x01, 0x00, 0x5B, 
0x5C, 0x36, 0xFF, 0xF8, 0x11, 0xFB, 0x79, 0xDE, 0x45, 0x62, 
0x95, 0x4D, 0xDF, 0x23, 0x5E, 0x52, 0x29, 0x7C, 0xE3, 0x97, 
0xBD, 0x4A, 0x1E, 0xA5, 0xBB, 0x2A, 0x19, 0xD4, 0x63, 0x4F, 
0xA6, 0x88, 0x9A, 0xA4, 0xCD, 0x1F, 0x87, 0xDC, 0x97, 0xC9, 
0x25, 0x9B, 0xD0, 0x8A, 0x55, 0x5B, 0xE7, 0xC6, 0x23, 0xCA, 
0xE6, 0x74, 0xBE, 0x66, 0xC2, 0xBA, 0x5F, 0xE3, 0x8F, 0xEC, 
0xA1, 0xA1, 0x49, 0x8F, 0xAC, 0xE1, 0x89, 0xFF, 0x93, 0xA6, 
0x0A, 0x37, 0x2B, 0x2D, 0xA8, 0xFB, 0xA1, 0xAB, 0x35, 0x76, 
0x59, 0x04, 0xDE, 0x14, 0xEC, 0x97, 0xD7, 0x15, 0x03, 0xB3, 
0xF2, 0xE5, 0xA8, 0x83, 0xDD, 0x55, 0x4A, 0x2F, 0x72, 0x48, 
0xCF, 0x29, 0x5A, 0xA8, 0xE6, 0x6C, 0xAE, 0xFB, 0x34, 0x25, 
0x8F, 0x63, 0x32, 0xD6, 0xDE, 0x81, 0xB3, 0x97, 0x63, 0x5E, 
0xF7, 0x1C, 0xD5, 0x46, 0xFA, 0x00, 0x97, 0x40, 0x2C, 0x22, 
0xAD, 0xBD, 0xE4, 0xC5, 0x8E, 0x31, 0xE4, 0x73, 0x09, 0x28, 
0x8F, 0x81, 0xFF, 0xDC, 0xE7, 0xBF, 0xBC, 0x83, 0xBE, 0x81, 
0xB3, 0x54, 0x20, 0x5E, 0xE2, 0xA7, 0x2B, 0xB9, 0xD7, 0x3D, 
0xD0, 0x91, 0x34, 0x62, 0x00, 0x02, 0x84, 0x3A, 0xE1, 0x2B, 
0xA2, 0x76, 0x09, 0x5A, 0xA1, 0x48, 0xDC, 0xFE, 0x63, 0x8D, 
0xAC, 0x06, 0x55, 0x1B, 0xF6, 0x42, 0x83, 0xF6, 0xD6, 0x75, 
0x0B, 0x9A, 0x85, 0x67, 0x03, 0x93, 0x14, 0xE9, 0x2D, 0x40, 
0x63, 0xE8, 0x76, 0xEA, 0xCE, 0x90, 0x6C, 0x5F, 0x71, 0xEC, 
0x92, 0xB3, 0xF0, 0x1C, 0xEB, 0x27, 0xF5, 0x31, 0x37, 0x64, 
0x56, 0x7D, 0xE8, 0xBC, 0x43, 0xA5, 0x9A, 0xED, 0x66, 0xCA, 
0xBF, 0x93, 0xBC, 0x3D, 0x58, 0x62, 0xCD, 0x04, 0xD1, 0xF1, 
0x8E, 0x20, 0xDA, 0x4C, 0xA2, 0xB8, 0x3E, 0xB7, 0x2E, 0x7A, 
0x54, 0xE6, 0xDD, 0xDD, 0x34, 0x49, 0x52, 0xDF, 0x4E, 0xD1, 
0x60, 0x86, 0x02, 0x18, 0xBB, 0xC4, 0x1C, 0x4B, 0x48, 0x71, 
0x9D, 0x9D, 0x40, 0x88, 0xE5, 0xF5, 0xB0, 0x29, 0x29, 0xC4, 
0x95, 0xB2, 0x7E, 0xC4, 0xEF, 0x38, 0x90, 0xBF, 0xC2, 0x6B, 
0x09, 0xC6, 0xC1, 0x93, 0x94, 0x87, 0x49, 0x69, 0x6D, 0x69, 
0x57, 0x5F, 0xAE, 0x4B, 0xE4, 0xF8, 0x1A, 0x9D, 0xC9, 0x8D, 
0x4A, 0xF4, 0x34, 0x6D, 0x75, 0xDB, 0x24, 0xC2, 0x97, 0x2A, 
0xA1, 0x76, 0x75, 0x76, 0x8A, 0x7C, 0x15, 0xAE, 0xAE, 0xEE, 
0x09, 0x91, 0x54, 0xAF, 0x75, 0x22, 0x1F, 0x84, 0xF9, 0xFA, 
0x70, 0x5B, 0xD5, 0x3D, 0x95, 0x17, 0x35, 0x5D, 0xFB, 0xC4, 
0xBF, 0xF1, 0x16, 0x10, 0xD5, 0xCF, 0x42, 0xDF, 0xFC, 0x5C, 
0xCB, 0xE5, 0xF0, 0x23, 0x6E, 0xC4, 0xBA, 0xF1, 0x99, 0xC6, 
0x56, 0x81, 0x4D, 0x09, 0xD1, 0x4A, 0x8A, 0x20, 0x54, 0x0F, 
0xB5, 0xCE, 0x92, 0x39, 0x5A, 0x0D, 0x3C, 0x84, 0x31, 0x20, 
0xD1, 0xE6, 0x81, 0xBA, 0xB8, 0x78, 0xA8, 0x94, 0xC5, 0x59, 
0x0B, 0xAB, 0x55, 0x50, 0x5D, 0xD5, 0x49, 0xC8, 0xE7, 0xB8, 
0x39, 0x0A, 0x06, 0x6D, 0x5A, 0xBE, 0x2C, 0x44, 0x13, 0x19, 
0x76, 0x22, 0xB0, 0x35, 0xF7, 0x59, 0x1B, 0xD6, 0xC9, 0xBC, 
0x35, 0x31, 0x96, 0x77, 0x34, 0xA6, 0x90, 0x5C, 0x9D, 0x49, 
0x14, 0x77, 0xCF, 0x79, 0xF6, 0x59, 0x77, 0xD2, 0x5A, 0x61, 
0x58, 0xDB, 0x52, 0xF1, 0x8C, 0x04, 0xA6, 0xF0, 0xE8, 0x95, 
0xEC, 0xDB, 0x3D, 0xA0, 0x6E, 0x3D, 0xEF, 0x89, 0x98, 0xE6, 
0xBF, 0xC6, 0xDE, 0x99, 0xD6, 0x61, 0x33, 0xDC, 0xEE, 0x85, 
0x9C, 0x64, 0x4A, 0x24, 0xDD, 0xAC, 0xDA, 0xA5, 0xDF, 0x97, 
0x2F, 0x4E, 0xCF, 0x54, 0x52, 0x76, 0x4A, 0xE9, 0x19, 0x3F, 
0x8F, 0x3A, 0xB6, 0xB9, 0xA5, 0xDC, 0xF2, 0x84, 0xBE, 0x40, 
0x57, 0x11, 0x3C, 0xD2, 0xBF, 0xB9, 0x6F, 0x74, 0x62, 0x3C, 
0x32, 0xB6, 0xC7, 0xF1, 0x07, 0x9E, 0xFE, 0x43, 0xA5, 0xB8, 
0x1A, 0x65, 0x9F, 0xE4, 0xDF, 0x1F, 0x4E, 0x42, 0x9A, 0x6C, 
0xDC, 0x2A, 0x8F, 0x80, 0x05, 0xF0, 0x5E, 0x60, 0x36, 0xC0, 
0x03, 0x82, 0x07, 0xF9, 0x47, 0xB2, 0xE8, 0x44, 0x70, 0x5D, 
0xFE, 0xBC, 0x5C, 0xAA, 0xFA, 0x04, 0x17, 0x0C, 0xD0, 0x63, 
0xCB, 0xF6, 0xA5, 0x0D, 0xB5, 0x14, 0xDA, 0xEA, 0x25, 0x62, 
0x35, 0x59, 0x2B, 0x83, 0x96, 0xA2, 0x60, 0x06, 0xD0, 0xE6, 
0x6B, 0x6D, 0xE9, 0x8B, 0x9C, 0x93, 0x62, 0x4F, 0x92, 0x66, 
0xB9, 0x67, 0x04, 0x92, 0x07, 0xB3, 0xAE, 0x0D, 0x31, 0x12, 
0x25, 0x1D, 0x31, 0x60, 0x23, 0x11, 0xB0, 0x03, 0x61, 0x08, 
0x5D, 0xB7, 0x73, 0x01, 0x5D, 0xFD, 0xB0, 0x69, 0x80, 0xBE, 
0x18, 0xA7, 0x99, 0x11, 0x48, 0x07, 0xB2, 0xEA, 0xF1, 0x14, 
0x82, 0x3C, 0xB1, 0xC3, 0x44, 0x75, 0xB1, 0xBD, 0x06, 0x09, 
0x31, 0x2A, 0x80, 0xAF, 0x54, 0x3B, 0x66, 0x83, 0xAC, 0x36, 
0xC1, 0xCB, 0x34, 0x9E, 0x13, 0x62, 0x6E, 0xDE, 0xAA, 0x10, 
0x1A, 0xB5, 0x8B, 0xFD, 0x33, 0x9B, 0xAC, 0xCE, 0xF5, 0x43, 
0xDF, 0xDE, 0x7A, 0x5B, 0x50, 0xC8, 0x9C, 0x7F, 0x93, 0x1B, 
0xBB, 0x2A, 0x02, 0x17, 0x03, 0x2D, 0xE2, 0x60, 0xB4, 0x62, 
0x1A, 0xAF, 0x5B, 0x00, 0x5C, 0x83, 0xE8, 0xBE, 0xFC, 0x47, 
0x9E, 0xAE, 0x37, 0xB4, 0xF8, 0xA6, 0x3B, 0x22, 0x6B, 0x89, 
0x87, 0xDF, 0x5D, 0x9B, 0xF9, 0x59, 0xCA, 0xBD, 0x10, 0x35, 
0xA8, 0x84, 0x7F, 0x4E, 0x28, 0x88, 0xAD, 0x0C, 0x10, 0x5A, 
0x39, 0x3E, 0x22, 0x8F, 0x08, 0x8F, 0x3F, 0x62, 0xEE, 0x71, 
0x09, 0x55, 0x7B, 0xF4, 0x63, 0xA0, 0xFD, 0x45, 0x91, 0x35, 
0xFF, 0xF3, 0xDE, 0x5B, 0x98, 0xB9, 0x06, 0x74, 0x06, 0xEF, 
0x3E, 0xF2, 0x2B, 0xD4, 0x09, 0x8B, 0xF9, 0x96, 0x63, 0x81, 
0x6B, 0xEA, 0x51, 0xF3, 0xA9, 0x1C, 0x35, 0x95, 0xBB, 0x6B, 
0x91, 0x65, 0x52, 0xEA, 0xEF, 0xBF, 0xAB, 0xD4, 0x56, 0x3F, 
0xB5, 0x40, 0x6D, 0xCC, 0x04, 0xC0, 0x32, 0x4A, 0xFE, 0x59, 
0xCD, 0x76, 0x8A, 0x9B, 0x5F, 0xDB, 0xE0, 0xD6, 0xFD, 0x2A, 
0xA2, 0xA8, 0x2A, 0x31, 0x83, 0xF1, 0x43, 0x4D, 0x42, 0x92, 
0xF4, 0xFB, 0x0A, 0x9E, 0x69, 0xF1, 0x02, 0xCD, 0xFD, 0xF8, 
0xD2, 0x59, 0xCC, 0x05, 0x66, 0x60, 0x17, 0x07, 0xD4, 0x2A, 
0x79, 0xD3, 0xF5, 0x34, 0xE0, 0xBD, 0x5D, 0x34, 0xAD, 0x76, 
0x37, 0x7D, 0x96, 0x08, 0xEC, 0x45, 0x82, 0xC0, 0xBD, 0x89, 
0x1F, 0xA5, 0x82, 0x08, 0x33, 0x1D, 0x6E, 0x70, 0x6D, 0xF2, 
0x4D, 0xBF, 0xBD, 0xC8, 0x4F, 0x44, 0xB9, 0x06, 0xF3, 0x31, 
0xD0, 0x40, 0x0D, 0x64, 0x04, 0x61, 0x54, 0x5C, 0x8F, 0xAA, 
0x9E, 0x05, 0xE3, 0x47, 0xF4, 0xC0, 0x94, 0x6B, 0x64, 0x81, 
0x7D, 0x2F, 0x96, 0xA5, 0xAF, 0xE5, 0xD7, 0x25, 0x91, 0x48, 
0xA4, 0xEC, 0xD4, 0x83, 0x19, 0x15, 0x9A, 0x00, 0x0D, 0xCA, 
0x66, 0xF6, 0xD4, 0xF8, 0x23, 0x57, 0xCA, 0x7E, 0x2C, 0xE2, 
0x1F, 0x8E, 0x75, 0x23, 0x3F, 0x2E, 0x4A, 0xAA, 0xD3, 0xC9, 
0xD7, 0xD7, 0xFE, 0x2A, 0xE3, 0xDC, 0x73, 0x07, 0x89, 0x64, 
0xBE, 0x7A, 0xB4, 0xFE, 0x11, 0x7C, 0x13, 0xCF, 0x02, 0x92, 
0xF7, 0x4A, 0x8C, 0xA5, 0x47, 0xC8, 0x5B, 0x2E, 0xCC, 0x5C, 
0x01, 0x23, 0xBE, 0xEA, 0x8C, 0x16, 0x7E, 0x2D, 0x88, 0x45, 
0x03, 0xA2, 0xC6, 0x5E, 0x8A, 0x6F, 0x19, 0x72, 0xE6, 0x43, 
0x69, 0x53, 0x76, 0xFC, 0x96, 0xBA, 0x29, 0x1C, 0x90, 0x02, 
0x88, 0x81, 0xBD, 0x6B, 0x3D, 0xBB, 0xDB, 0x30, 0xB9, 0xF9, 
0x1B, 0x59, 0x92, 0x20, 0xE3, 0xA7, 0xCD, 0x0B, 0x97, 0xB8, 
0xA7, 0x45, 0xE0, 0x56, 0x91, 0x29, 0x44, 0x65, 0xE5, 0x20, 
0x6A, 0xC3, 0xA4, 0x77, 0x72, 0x6D, 0xDF, 0x08, 0xD5, 0xBA, 
0x6A, 0xEB, 0xAD, 0x27, 0x52, 0xDA, 0xA0, 0xEE, 0xE6, 0x6A, 
0xFA, 0x07, 0x73, 0xEE, 0xE0, 0xCC, 0x7F, 0xC4, 0x2B, 0xF3, 
0x1A, 0xB8, 0x8A, 0xF6, 0xCD, 0x2D, 0x8F, 0x8B, 0x04, 0xA4, 
0x1E, 0x8B, 0xFD, 0x70, 0x9A, 0xE2, 0xA4, 0xA0, 0x9F, 0xEE, 
0x42, 0x65, 0xF2, 0x99, 0x2A, 0x3A, 0x62, 0xF3, 0x00, 0x83, 
0xE6, 0x5F, 0xE8, 0x60, 0x13, 0xC0, 0x7E, 0x67, 0x97, 0x37, 
0x0E, 0x35, 0x83, 0x0D, 0x83, 0xE8, 0x8E, 0x15, 0xF9, 0x55, 
0x96, 0x69, 0x9F, 0x19, 0x16, 0xBA, 0x41, 0x0A, 0x74, 0xA1, 
0xD5, 0x41, 0xA2, 0x4F, 0x47, 0x54, 0x3A, 0xCB, 0xCE, 0xAF, 
0xA6, 0xFB, 0xB6, 0x76, 0x36, 0x40, 0x1F, 0x48, 0x7A, 0x4D, 
0x1D, 0xB0, 0xBE, 0x60, 0x24, 0xFA, 0x10, 0xE0, 0xD2, 0x44, 
0xB9, 0xEE, 0x92, 0xFE, 0xF5, 0x4A
};
//1E0015F0
static int xcode_building_first_stage(struct cxdec_xcode_status *xcode)
{
      switch (xcode_rand(xcode) % 3) {
      case 1:
        // MOV ESI, EncryptionControlBlock
        // MOV EAX, DWORD PTR DS:[ESI+((xcode_rand(xcode) & 0x3ff) << 2)]
        if (!push_bytexcode(xcode, 0xbe)
                || !push_dwordxcode(xcode, (DWORD)EncryptionControlBlock)
                || !push_2bytesxcode(xcode, 0x8b, 0x86)
                || !push_dwordxcode(xcode, (xcode_rand(xcode) & 0x3ff) << 2))
            return 0;
        break;
    case 2:
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
// 1E0016B0
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
    case 2:
        // NOT EAX
        if (!push_2bytesxcode(xcode, 0xf7, 0xd0))
            return 0;
        break;
    case 7:
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
    case 5:
        // DEC EAX
        if (!push_bytexcode(xcode, 0x48))
            return 0;
        break;
    case 3:
        // NEG EAX
        if (!push_2bytesxcode(xcode, 0xf7, 0xd8))
            return 0;
        break;
    case 6:
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
    case 4:
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
    case 1:
        // INC EAX
        if (!push_bytexcode(xcode, 0x40))
            return 0;
        break;
    case 0:
        // XOR EAX, xcode_rand(xcode)
        if (!push_bytexcode(xcode, 0x35) 
                || !push_dwordxcode(xcode, xcode_rand(xcode)))
            return 0;
        break;
    }
    return 1;
}
// 0x1E001900
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
    case 1:
        // ADD EAX, EBX
        if (!push_2bytesxcode(xcode, 0x01, 0xd8))
            return 0;
        break;
    case 4:
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
    case 3:
        // NEG EAX, ADD EAX, EBX
        if (!push_2bytesxcode(xcode, 0xf7, 0xd8) 
                || !push_2bytesxcode(xcode, 0x01, 0xd8))
            return 0;
        break;
    case 5:
        // IMUL EAX, EBX
        if (!push_3bytesxcode(xcode, 0x0f,  0xaf, 0xc3))
            return 0;
        break;
    case 2:
        // SUB EAX, EBX
        if (!push_2bytesxcode(xcode, 0x29, 0xd8))
            return 0;
        break;
    }
    // POP EBX
    return push_bytexcode(xcode, 0x5b);
}

struct cxdec_callback sukisuki_cxdec_callback = {
    "sukisuki",
    { 0x22B, 0x6C8},
    xcode_building_stage1
};