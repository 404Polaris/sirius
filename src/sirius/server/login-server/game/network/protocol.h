//
// Created by 404Polaris on 2019/10/15.
//

#pragma once

#include <cstdint>
#include <cstddef>

#pragma pack(push, 1)

typedef struct {
	uint8_t cmd;
	uint8_t error;
	uint16_t size;
	uint8_t game_name[4];
	uint8_t version1;
	uint8_t version2;
	uint8_t version3;
	uint16_t build;
	uint8_t platform[4];
	uint8_t os[4];
	uint8_t country[4];
	uint32_t timezone_bias;
	uint32_t ip;
	uint8_t i_len;
	uint8_t i[1];
} s_auth_logon_challenge_c;

typedef struct {
	uint8_t cmd;
	uint8_t a[32];
	uint8_t m1[20];
	uint8_t crc_hash[20];
	uint8_t number_of_keys;
	uint8_t security_flags;
} s_auth_logon_proof_c;

typedef struct {
	uint8_t cmd;
	uint8_t error;
	uint8_t m2[20];
	uint32_t account_flags;
	uint32_t survey_id;
	uint16_t login_flags;
} s_auth_logon_proof_s;

typedef struct {
	uint8_t cmd;
	uint8_t error;
	uint8_t m2[20];
	uint32_t unk2;
} s_auth_logon_proof_s_old;

typedef struct {
	uint8_t cmd;
	uint8_t r1[16];
	uint8_t r2[20];
	uint8_t r3[20];
	uint8_t number_of_keys;
} s_auth_reconnect_proof_c;

typedef struct {
	uint8_t cmd;
} s_realm_list_c;

#pragma pack(pop)

enum class login_server_cmd : uint8_t {
	kAuthLogonChallenge = 0x00,
	kAuthLogonProof = 0x01,
	kAuthReconnectChallenge = 0x02,
	kAuthReconnectProof = 0x03,
	kRealmList = 0x10,
};

