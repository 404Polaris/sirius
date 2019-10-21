//
// Created by 404Polaris on 2019/10/21.
//


#pragma once

enum class AuthStatus : uint8_t {
	kStatusChallenge = 0,
	kStatusLogonProof,
	kStatusReconnectProof,
	kStatusAuthed,
	kStatusWaitingForRealmList,
	kStatusClosed
};