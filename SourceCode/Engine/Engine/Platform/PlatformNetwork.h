// Copyright 2016-2017 ?????????????. All Rights Reserved.
#pragma once
#include <Common\PrimitiveTypes.h>
#include <utility>

#ifndef PLATFORM_NETWORK_H
#define PLATFORM_NETWORK_H

namespace Engine
{
	using namespace Common;

	namespace Platform
	{
		//
		// http://gafferongames.com/networking-for-game-programmers/sending-and-receiving-packets/
		//
		class PLATFORM_API PlatformNetwork
		{
		public:
			enum class AddressFamilies
			{
				InterNetwork
			};

			enum class Types
			{
				Stream,
				Datagram,
				RawProtocol,
				ReliablyDeliverdMessage,
				SequenedPacketStream
			};

			enum class IPProtocols
			{
				UDP
			};

			enum class SendModes
			{
				None,
				DontRoute,
				OutOfBand
			};

			enum class Errors
			{
				NoError,
				BaseError,
				Interrupted,
				BadFile,
				AccessDenied,
				InvalidPointer,
				InvalidArguments,
				TooManySockets,
				WouldBlock,
				BlockingInProgress,
				NonBlockingInProgress,
				NoSocket,
				DestinationAddressRequired,
				LargePacketSize,
				MismatchProtocolType,
				NoProtocolOperation,
				ProtocolNotSupported,
				SocketNotSupported,
				OperationNotSupported,
				ProtocolFamilyNotSupported,
				AddressFamilyNotSupported,
				AddressInUse,
				AddressNotValid,
				NetworkDown,
				NetworkUnreachable,
				NetworkReset,
				ConnectionAbort,
				ConnectionReset,
				NoBuffer,
				IsConnected,
				NotConnected,
				Shutdown,
				TooManyReferences,
				Timeout,
				ConnectionRefused,
				Loop,
				NameTooLong,
				HostDown,
				HostUnreachable,
				NotEmpty,
				ProcessLimit,
				OutOfUsers,
				OutOfDisk,
				HandleNotExists,
				ItemNotExists,
				SystemNotReady,
				VersionNotSupported,
				NotInitialized,
				Disconnected,
				InvalidProcedureTable,
				InvalidProvider,
				ProviderFailedToInitialize,
				SystemCallFailure,
				ServiceNotFound,
				TypeNotFound,
				NoMoreResult2,
				Canceled2,
				Refused,
				HostNotFound,
				TryAgain,
				NoRecoverable,
				NoData,
				QOSReceivers,
				QOSNoSenders,
				QOSNoReceiver,
				QOSRequestConfirmed,
				AdmissionFailure,
				QOSPolicyFailure,
				QOSBadStyle,
				QOSBadObject,
				QOSTrafficControlError,
				QOSGenericError,
				QOSServiceType,
				QOSFlowSpecific,
				QOSProviderSpecific,
				QOSFilterStyle,
				QOSFilterType,
				QOSFilterCount,
				QOSObjectLength,
				QOSLowCount,
				QOSUnknownProviderSpecific,
				QOSPolicyObject,
				QOSFlowDescriptor,
				QOSInconsistentFlowSpecific,
				QOSInconsistentFilterSpecific,
				QOSShapeDiscardMode,
				QOSShapingRateObject,
				QOSReceivedProviderType,
				QOSSecureHostNotFound,
				IPSecPolicy,
			};

			typedef uint32 Handle;

			static bool Initialize(void);
			static bool Shotdown(void);

			static Handle Create(AddressFamilies AddressFamily, Types Type, IPProtocols IPProtocol);
			static bool Close(Handle Handle);

			static bool Bind(Handle Handle, AddressFamilies AddressFamily, uint16 Port);

			static bool SetNonBlocking(Handle Handle, bool Enabled);

			static bool Send(Handle Handle, const byte *Buffer, uint32 Length, AddressFamilies AddressFamily, uint16 Port, SendModes Mode);

			static Errors GetLastError(void);

			static uint32 GetAddress(uint8 A, uint8 B, uint8 C, uint8 D)
			{
				return ((A << 24) | (B << 16) | (C << 8) | D);
			}
		};
	}
}

#endif