#include "stdafx.h"
#include "FLVParser.h"
#include "amf.h"

namespace Decode
{
	FLVParser::FLVParser()
		:m_hFile(NULL),
		m_lengthSizeMinusOne(4),
		m_bAudio(FALSE),
		m_bVideo(FALSE),
		m_dts(0),
		m_index(0)
	{

	}
	FLVParser::~FLVParser()
	{
		if (m_hFile != NULL)
		{
			fclose(m_hFile);
			m_hFile = NULL;
		}
	}
	BOOL FLVParser::Open(LPCSTR pzFile)
	{
		Close();
		fopen_s(&m_hFile, pzFile, "rb");
		return m_hFile != NULL;
	}
	BOOL FLVParser::Close()
	{
		if (m_hFile != NULL)
		{
			fclose(m_hFile);
			m_hFile = NULL;
		}
		return TRUE;
	}
	BOOL FLVParser::ParseVideo(BYTE* data, INT size)
	{
		BOOL bRes = FALSE;
		FLV_TAG_VIDEO* video = reinterpret_cast<FLV_TAG_VIDEO*>(data);
		switch (video->codeID)
		{
		case FLV_CODECID_H264:
			bRes = ParseH264(video, data + 1, size - 1);
			break;
		case FLV_CODECID_MPEG4:
			bRes = ParseMPEG4(video, data + 1, size - 1);
			break;
		case FLV_CODECID_H263:
		case FLV_CODECID_SCREEN:
		case FLV_CODECID_VP6:
		case FLV_CODECID_VP6A:
		case FLV_CODECID_SCREEN2:
		case FLV_CODECID_REALH263:
			break;
		}
		return TRUE;
	}
	BOOL FLVParser::ParseAudio(BYTE* data, INT size)
	{
		BOOL bRes = FALSE;
		//Ŀǰֻ֧��MP3,AAC��PCM
		FLV_TAG_AUDIO* audio = reinterpret_cast<FLV_TAG_AUDIO*>(data);
		switch (audio->codeID)
		{
		case FLV_CODECID_PCM:
			bRes = ParsePCM(audio, data + 1, size - 1);
			break;
		case FLV_CODECID_AAC:
			bRes = ParseAAC(audio, data + 1, size - 1);
			break;
		case FLV_CODECID_MP3:
			bRes = ParseMP3(audio, data + 1, size - 1);
			break;
		case FLV_CODECID_SPEEX:
		case FLV_CODECID_ADPCM:
		case FLV_CODECID_PCM_LE:
		case FLV_CODECID_NELLYMOSER_16KHZ_MONO:
		case FLV_CODECID_NELLYMOSER_8KHZ_MONO:
		case FLV_CODECID_NELLYMOSER:
		case FLV_CODECID_PCM_ALAW:
		case FLV_CODECID_PCM_MULAW:
			break;
		}
		return bRes;
	}
	BOOL FLVParser::ParseScript(BYTE* data, INT size)
	{
		AMFObject metaObj;
		if (AMF_Decode(&metaObj, reinterpret_cast<CHAR*>(data), size, FALSE) > 0)
		{
			FLV_SCRIPTDATA	script = { 0 };
			for (INT i = 0; i < metaObj.o_num; i++)
			{
				string val;
				AMFObjectProperty* prop = &metaObj.o_props[i];
				if (prop->p_type == AMF_OBJECT || prop->p_type == AMF_ECMA_ARRAY)
				{
					for (INT j = 0; j < prop->p_vu.p_object.o_num; j++)
					{
						AMFObjectProperty * objProp = &prop->p_vu.p_object.o_props[j];
						if (objProp->p_name.av_len > 0)
						{
							val.resize(objProp->p_name.av_len);
							val = objProp->p_name.av_val;
							if ("width" == val)
							{
								ASSERT(objProp->p_type == AMF_NUMBER);
								script.width = objProp->p_vu.p_number;
							}
							if ("height" == val)
							{
								ASSERT(objProp->p_type == AMF_NUMBER);
								script.height = objProp->p_vu.p_number;
							}
							if ("duration" == val)
							{
								ASSERT(objProp->p_type == AMF_NUMBER);
								script.duration = objProp->p_vu.p_number * 1000000;
							}
							if ("framerate" == val)
							{
								ASSERT(objProp->p_type == AMF_NUMBER);
								script.framerate = objProp->p_vu.p_number;
							}
							if ("filesize" == val)
							{
								ASSERT(objProp->p_type == AMF_NUMBER);
								script.filesize = objProp->p_vu.p_number;
							}
							if ("datasize" == val)
							{
								ASSERT(objProp->p_type == AMF_NUMBER);
								script.datasize = objProp->p_vu.p_number;
							}
							if ("audiosize" == val)
							{
								ASSERT(objProp->p_type == AMF_NUMBER);
								script.audiosize = objProp->p_vu.p_number;
							}
							if ("videosize" == val)
							{
								ASSERT(objProp->p_type == AMF_NUMBER);
								script.videosize = objProp->p_vu.p_number;
							}
							if ("videocodecid" == val)
							{
								ASSERT(objProp->p_type == AMF_NUMBER);
								script.videocodecid = objProp->p_vu.p_number;
							}
							if ("videodatarate" == val)
							{
								ASSERT(objProp->p_type == AMF_NUMBER);
								script.videodatarate = objProp->p_vu.p_number;
							}
							if ("audiocodecid" == val)
							{
								ASSERT(objProp->p_type == AMF_NUMBER);
								script.audiocodecid = objProp->p_vu.p_number;
							}
							if ("audiodatarate" == val)
							{
								ASSERT(objProp->p_type == AMF_NUMBER);
								script.audiodatarate = objProp->p_vu.p_number;
							}
							if ("audiosamplesize" == val)
							{
								ASSERT(objProp->p_type == AMF_NUMBER);
								script.audiosamplesize = objProp->p_vu.p_number;
							}
							if ("audiosamplerate" == val)
							{
								ASSERT(objProp->p_type == AMF_NUMBER);
								script.audiosamplerate = objProp->p_vu.p_number;
							}
						}
					}
				}
			}
			AMF_Reset(&metaObj);
			EVENT_SCRIPT(&script);
		}
		return TRUE;
	}
	BOOL FLVParser::ParseMPEG4(FLV_TAG_VIDEO* video, BYTE* data, INT size)
	{
		return TRUE;
	}
	BOOL FLVParser::ParseH264(FLV_TAG_VIDEO* video, BYTE* data, INT size)
	{
		BYTE* bits = data;
		BYTE aacPacketType = *bits++;
		size -= 1;
		INT cts = (ToINT24(bits) + 0xFF800000) ^ 0xFF800000;
		bits += 3;
		size -= 3;
		if (aacPacketType == 0)
		{
			TinyBufferArray<BYTE> buffer;
			AVCDecoderConfigurationRecord avcconfig;
			avcconfig.ConfigurationVersion = *bits++;
			avcconfig.AVCProfileIndication = *bits++;
			avcconfig.ProfileCompatibility = *bits++;
			avcconfig.AVCLevelIndication = *bits++;
			avcconfig.LengthSizeMinusOne = *bits++ & 0x03 + 1;//һ����4
			avcconfig.NumOfSequenceParameterSets = *bits++ & 0x1F;//һ����1
			m_lengthSizeMinusOne = avcconfig.LengthSizeMinusOne;
			for (INT i = 0; i < avcconfig.NumOfSequenceParameterSets; i++)
			{
				BYTE* val = reinterpret_cast<BYTE*>(const_cast<UINT32*>(&H264StartCode));
				buffer.Add(val, 4);
				USHORT s = *bits++ << 8;
				s |= *bits++;
				buffer.Add(bits, s);
				bits += s;
			}
			avcconfig.NumOfPictureParameterSets = *bits++;
			for (INT i = 0; i < avcconfig.NumOfPictureParameterSets; i++)
			{
				buffer.Add(reinterpret_cast<BYTE*>(const_cast<UINT32*>(&H264StartCode)), 4);
				USHORT s = *bits++ << 8;
				s |= *bits++;
				buffer.Add(bits, s);
				bits += s;
			}
			FLV_PACKET packet;
			packet.dts = m_dts;
			packet.pts = m_dts + cts;
			packet.codeID = video->codeID;
			packet.codeType = video->codeType;
			packet.packetType = FLV_AVCDecoderConfigurationRecord;
			EVENT_VIDEO(buffer.GetPointer(), buffer.GetSize(), &packet);
		}
		if (aacPacketType == 1)
		{
			return ParseNALU(video, &cts, bits, size);
		}
		return TRUE;
	}
	BOOL FLVParser::ParseNALU(FLV_TAG_VIDEO* video, INT* cts, BYTE* data, INT size)
	{
		BYTE* bits = data;
		INT offset = 0;
		INT sizeofNALU = 0;
		FLV_PACKET packet = { 0 };
		for (;;)
		{
			if (offset >= size)
				break;
			switch (m_lengthSizeMinusOne)
			{
			case 4:
			{
				sizeofNALU = ToINT32(bits);
				bits += 4;
				offset += 4;
				TinyScopedPtr<BYTE> val(new BYTE[sizeofNALU + 4]);
				memcpy(val, &H264StartCode, 4);
				memcpy(val + 4, bits, sizeofNALU);
				packet.codeID = video->codeID;
				packet.codeType = video->codeType;
				packet.packetType = FLV_NALU;
				packet.dts = m_dts;
				packet.pts = m_dts + *cts;
				//TRACE("video-pts:%d\n", packet.pts);
				EVENT_VIDEO(val, sizeofNALU + 4, &packet);
				bits += sizeofNALU;
				offset += sizeofNALU;
			}
			break;
			case 3:
			{
				sizeofNALU = ToINT24(bits);
				bits += 3;
				offset += 3;
				TinyScopedPtr<BYTE> val(new BYTE[sizeofNALU + 4]);
				memcpy(val, &H264StartCode, 4);
				memcpy(val + 4, bits, sizeofNALU);
				packet.codeID = video->codeID;
				packet.codeType = video->codeType;
				packet.packetType = FLV_NALU;
				packet.dts = m_dts;
				packet.pts = m_dts + *cts;
				EVENT_VIDEO(val, sizeofNALU + 4, &packet);
				bits += sizeofNALU;
				offset += sizeofNALU;
			}
			break;
			case 2:
			{
				sizeofNALU = ToINT16(bits);
				bits += 2;
				offset += 2;
				TinyScopedPtr<BYTE> val(new BYTE[sizeofNALU + 4]);
				memcpy(val, &H264StartCode, 4);
				memcpy(val + 4, bits, sizeofNALU);
				packet.codeID = video->codeID;
				packet.codeType = video->codeType;
				packet.packetType = FLV_NALU;
				packet.dts = m_dts;
				packet.pts = m_dts + *cts;
				EVENT_VIDEO(val, sizeofNALU + 4, &packet);
				bits += sizeofNALU;
				offset += sizeofNALU;
			}
			break;
			case 1:
			{
				sizeofNALU = ToINT8(bits);
				bits += 1;
				offset += 1;
				TinyScopedPtr<BYTE> val(new BYTE[sizeofNALU + 4]);
				memcpy(val, &H264StartCode, 4);
				memcpy(val + 4, bits, sizeofNALU);
				packet.codeID = video->codeID;
				packet.codeType = video->codeType;
				packet.packetType = FLV_NALU;
				packet.dts = m_dts;
				packet.pts = m_dts + *cts;
				EVENT_VIDEO(val, sizeofNALU + 4, &packet);
				bits += sizeofNALU;
				offset += sizeofNALU;
			}
			break;
			}
		}
		return TRUE;
	}
	BOOL FLVParser::ParseAAC(FLV_TAG_AUDIO* audio, BYTE* data, INT size)
	{
		ASSERT(size >= 2);
		BYTE* bits = data;
		BYTE aacPacketType = *bits++;
		size -= 1;
		FLV_PACKET packet = { 0 };
		if (aacPacketType == 0)
		{
			packet.dts = m_dts;
			packet.pts = m_dts;
			//TRACE("audio-pts:%d\n", packet.pts);
			packet.bitsPerSample = audio->bitsPerSample;
			packet.channel = audio->channel;
			packet.codeID = FLV_CODECID_AAC;
			packet.packetType = FLV_AudioSpecificConfig;
			EVENT_AUDIO(bits, size, &packet);
		}
		if (aacPacketType == 1)
		{
			packet.dts = m_dts;
			packet.pts = m_dts;
			//TRACE("audio-pts:%d\n", packet.pts);
			packet.bitsPerSample = audio->bitsPerSample;
			packet.channel = audio->channel;
			packet.codeID = FLV_CODECID_AAC;
			packet.packetType = FLV_AACRaw;
			EVENT_AUDIO(bits, size, &packet);
		}
		return TRUE;
	}
	BOOL FLVParser::ParseMP3(FLV_TAG_AUDIO* audio, BYTE* data, INT size)
	{
		return TRUE;
	}
	BOOL FLVParser::ParsePCM(FLV_TAG_AUDIO* audio, BYTE* data, INT size)
	{
		return TRUE;
	}
	BOOL FLVParser::Parse()
	{
		FLV_HEADER header = { 0 };
		ASSERT(sizeof(FLV_HEADER) != fread(&header, sizeof(FLV_HEADER), 1, m_hFile));
		if (strncmp("FLV", (CHAR*)header.signature, 3) != 0)
			return FALSE;
		m_bAudio = (header.streamType & 0x04) != 0;
		m_bVideo = (header.streamType & 0x01) != 0;
		INT offset = ToINT32(header.offset);
		fseek(m_hFile, offset, SEEK_SET);
		for (;;)
		{
			UINT tagSize = 0;
			if (fread(&tagSize, sizeof(UINT), 1, m_hFile) <= 0)
				break;
			tagSize = htonl(tagSize);
			FLV_TAG_HEADER tag = { 0 };
			if (fread(&tag, sizeof(FLV_TAG_HEADER), 1, m_hFile) <= 0)
				break;
			INT size = ToINT24(tag.size);
			if (size > 0)
			{
				TinyScopedArray<BYTE> data(new BYTE[size]);
				if (fread(data, size, 1, m_hFile) <= 0)
					break;
				switch (tag.type)
				{
				case FLV_AUDIO:
					m_dts = static_cast<LONGLONG>(static_cast<UINT32>(ToINT24(tag.timestamp) | (tag.timestampex << 24)));
					ParseAudio(data, size);
					break;
				case FLV_VIDEO:
					m_dts = static_cast<LONGLONG>(static_cast<UINT32>(ToINT24(tag.timestamp) | (tag.timestampex << 24)));
					ParseVideo(data, size);
					break;
				case FLV_SCRIPT:
					ParseScript(data, size);
					break;
				default:
					break;
				}
			}
		}
		return TRUE;
	}
	//////////////////////////////////////////////////////////////////////////
	FLVReader::FLVReader()
		:m_bAudio(FALSE),
		m_bVideo(FALSE),
		m_lengthSizeMinusOne(0),
		m_offset(0),
		m_naluOffset(0),
		m_naluPtr(NULL),
		m_dts(0)
	{

	}
	FLVReader::~FLVReader()
	{

	}
	FLV_SCRIPTDATA FLVReader::GetScript()
	{
		return m_script;
	}
	BOOL FLVReader::Open(LPCSTR pzFile)
	{
		HRESULT hRes = SHCreateStreamOnFileA(pzFile, STGM_READ | STGM_FAILIFTHERE, &m_stream);
		if (hRes != S_OK)
			return FALSE;
		FLV_HEADER header = { 0 };
		ULONG ls = 0;
		hRes = m_stream->Read(&header, sizeof(FLV_HEADER), &ls);
		if (hRes != S_OK || ls != sizeof(FLV_HEADER))
			return FALSE;
		if (strncmp("FLV", (CHAR*)header.signature, 3) != 0)
			return FALSE;
		m_bAudio = (header.streamType & 0x04) != 0;
		m_bVideo = (header.streamType & 0x01) != 0;
		m_offset = ToINT32(header.offset);
		LARGE_INTEGER dlibMove = { 0 };
		dlibMove.QuadPart = m_offset;
		hRes = m_stream->Seek(dlibMove, STREAM_SEEK_SET, NULL);
		if (hRes != S_OK)
			return FALSE;
		UINT tagSize = 0;
		hRes = m_stream->Read(&tagSize, sizeof(UINT), &ls);
		if (hRes != S_OK || ls <= 0)
			return FALSE;
		tagSize = htonl(tagSize);
		FLV_TAG_HEADER tag = { 0 };
		hRes = m_stream->Read(&tag, sizeof(FLV_TAG_HEADER), &ls);
		if (hRes != S_OK || ls <= 0)
			return FALSE;
		INT size = ToINT24(tag.size);
		if (size > 0)
		{
			TinyScopedArray<BYTE> data(new BYTE[size]);
			hRes = m_stream->Read(data, size, &ls);
			if (hRes != S_OK || ls <= 0)
				return FALSE;
			if (tag.type == FLV_SCRIPT)
			{
				if (!ParseScript(data, size, m_script))
					return FALSE;
				ULARGE_INTEGER libNew = { 0 };
				ZeroMemory(&dlibMove, sizeof(dlibMove));
				hRes = m_stream->Seek(dlibMove, STREAM_SEEK_CUR, &libNew);
				if (hRes != S_OK)
					return FALSE;
				m_offset = libNew.LowPart;
			}
		}
		return TRUE;
	}
	DWORD FLVReader::Seek(LONG offset, DWORD dwFlag)
	{
		LARGE_INTEGER dlibMove = { 0 };
		dlibMove.LowPart = offset;
		ULARGE_INTEGER libNew = { 0 };
		HRESULT hRes = m_stream->Seek(dlibMove, dwFlag, &libNew);
		if (hRes != S_OK)
			return FALSE;
		return libNew.LowPart;
	}
	BOOL FLVReader::ReadBlock(FLV_BLOCK& block)
	{
		if (m_nalus.GetSize() <= 0)
		{
			UINT tagSize = 0;
			ULONG ls = 0;
			HRESULT hRes = m_stream->Read(&tagSize, sizeof(UINT), &ls);
			if (hRes != S_OK || ls <= 0)
				return FALSE;
			tagSize = htonl(tagSize);
			FLV_TAG_HEADER tag = { 0 };
			hRes = m_stream->Read(&tag, sizeof(FLV_TAG_HEADER), &ls);
			if (hRes != S_OK || ls <= 0)
				return FALSE;
			INT size = ToINT24(tag.size);
			if (size > 0)
			{
				block.type = tag.type;
				TinyScopedArray<BYTE> data(new BYTE[size]);
				hRes = m_stream->Read(data, size, &ls);
				if (hRes != S_OK || ls <= 0)
					return FALSE;
				if (tag.type == FLV_AUDIO)
				{
					m_dts = static_cast<LONGLONG>(static_cast<UINT32>(ToINT24(tag.timestamp) | (tag.timestampex << 24)));
					return ParseAudio(data, size, block);
				}
				if (tag.type == FLV_VIDEO)
				{
					m_dts = static_cast<LONGLONG>(static_cast<UINT32>(ToINT24(tag.timestamp) | (tag.timestampex << 24)));
					return ParseVideo(data, size, block);
				}
			}
		}
		else
		{
			return ParseNALU(&m_videoTag, block);
		}
		return FALSE;
	}
	BOOL FLVReader::ParseVideo(BYTE* data, INT size, FLV_BLOCK& block)
	{
		BOOL bRes = FALSE;
		FLV_TAG_VIDEO* video = reinterpret_cast<FLV_TAG_VIDEO*>(data);
		m_videoTag = *video;
		switch (video->codeID)
		{
		case FLV_CODECID_H264:
			bRes = ParseH264(video, data + 1, size - 1, block);
			break;
		case FLV_CODECID_MPEG4:
			bRes = ParseMPEG4(video, data + 1, size - 1, block);
			break;
		case FLV_CODECID_H263:
		case FLV_CODECID_SCREEN:
		case FLV_CODECID_VP6:
		case FLV_CODECID_VP6A:
		case FLV_CODECID_SCREEN2:
		case FLV_CODECID_REALH263:
			break;
		}
		return TRUE;
	}
	BOOL FLVReader::ParseAudio(BYTE* data, INT size, FLV_BLOCK& block)
	{
		BOOL bRes = FALSE;
		//Ŀǰֻ֧��MP3,AAC��PCM
		FLV_TAG_AUDIO* audio = reinterpret_cast<FLV_TAG_AUDIO*>(data);
		switch (audio->codeID)
		{
		case FLV_CODECID_PCM:
			bRes = ParsePCM(audio, data + 1, size - 1, block);
			break;
		case FLV_CODECID_AAC:
			bRes = ParseAAC(audio, data + 1, size - 1, block);
			break;
		case FLV_CODECID_MP3:
			bRes = ParseMP3(audio, data + 1, size - 1, block);
			break;
		case FLV_CODECID_SPEEX:
		case FLV_CODECID_ADPCM:
		case FLV_CODECID_PCM_LE:
		case FLV_CODECID_NELLYMOSER_16KHZ_MONO:
		case FLV_CODECID_NELLYMOSER_8KHZ_MONO:
		case FLV_CODECID_NELLYMOSER:
		case FLV_CODECID_PCM_ALAW:
		case FLV_CODECID_PCM_MULAW:
			break;
		}
		return bRes;
	}
	BOOL FLVReader::ParseAAC(FLV_TAG_AUDIO* audio, BYTE* data, INT size, FLV_BLOCK& block)
	{
		if (size < 2)
			return FALSE;
		BYTE* bits = data;
		BYTE aacPacketType = *bits++;
		size -= 1;
		if (aacPacketType == 0)
		{
			block.dts = m_dts;
			block.pts = m_dts;
			block.audio.bitsPerSample = audio->bitsPerSample;
			block.audio.channel = audio->channel;
			block.audio.codeID = FLV_CODECID_AAC;
			block.audio.packetType = FLV_AudioSpecificConfig;
			block.audio.size = size;
			block.audio.data = new BYTE[size];
			memcpy(block.audio.data, bits, size);
		}
		if (aacPacketType == 1)
		{
			block.dts = m_dts;
			block.pts = m_dts;
			block.audio.bitsPerSample = audio->bitsPerSample;
			block.audio.channel = audio->channel;
			block.audio.codeID = FLV_CODECID_AAC;
			block.audio.packetType = FLV_AACRaw;
			block.audio.size = size;
			block.audio.data = new BYTE[size];
			memcpy(block.audio.data, bits, size);
		}
		return TRUE;
	}
	BOOL FLVReader::ParseMP3(FLV_TAG_AUDIO* audio, BYTE* data, INT size, FLV_BLOCK& block)
	{
		return TRUE;
	}
	BOOL FLVReader::ParsePCM(FLV_TAG_AUDIO* audio, BYTE* data, INT size, FLV_BLOCK& block)
	{
		return TRUE;
	}
	BOOL FLVReader::ParseH264(FLV_TAG_VIDEO* video, BYTE* data, INT size, FLV_BLOCK& block)
	{
		BYTE* bits = data;
		BYTE aacPacketType = *bits++;
		size -= 1;
		block.video.cts = (ToINT24(bits) + 0xFF800000) ^ 0xFF800000;
		bits += 3;
		size -= 3;
		if (aacPacketType == 0)
		{
			TinyBufferArray<BYTE> buffer;
			AVCDecoderConfigurationRecord avcconfig;
			avcconfig.ConfigurationVersion = *bits++;
			avcconfig.AVCProfileIndication = *bits++;
			avcconfig.ProfileCompatibility = *bits++;
			avcconfig.AVCLevelIndication = *bits++;
			avcconfig.LengthSizeMinusOne = *bits++ & 0x03 + 1;//һ����4
			avcconfig.NumOfSequenceParameterSets = *bits++ & 0x1F;//һ����1
			m_lengthSizeMinusOne = avcconfig.LengthSizeMinusOne;
			for (INT i = 0; i < avcconfig.NumOfSequenceParameterSets; i++)
			{
				BYTE* val = reinterpret_cast<BYTE*>(const_cast<UINT32*>(&H264StartCode));
				buffer.Add(val, 4);
				USHORT s = *bits++ << 8;
				s |= *bits++;
				buffer.Add(bits, s);
				bits += s;
			}
			avcconfig.NumOfPictureParameterSets = *bits++;
			for (INT i = 0; i < avcconfig.NumOfPictureParameterSets; i++)
			{
				buffer.Add(reinterpret_cast<BYTE*>(const_cast<UINT32*>(&H264StartCode)), 4);
				USHORT s = *bits++ << 8;
				s |= *bits++;
				buffer.Add(bits, s);
				bits += s;
			}
			block.dts = m_dts;
			block.pts = block.dts + block.video.cts;
			block.video.codeID = video->codeID;
			block.video.codeType = video->codeType;
			block.video.packetType = FLV_AVCDecoderConfigurationRecord;
			block.video.size = buffer.GetSize();
			block.video.data = new BYTE[block.video.size];
			memcpy(block.video.data, buffer.GetPointer(), block.video.size);
		}
		if (aacPacketType == 1)
		{
			m_nalus.Clear();
			m_nalus.Add(bits, size);
			m_naluPtr = m_nalus;
			m_naluOffset = 0;
			return ParseNALU(video, block);
		}
		return TRUE;
	}
	BOOL FLVReader::ParseMPEG4(FLV_TAG_VIDEO* video, BYTE* data, INT size, FLV_BLOCK& block)
	{
		return TRUE;
	}
	BOOL FLVReader::ParseNALU(FLV_TAG_VIDEO* video, FLV_BLOCK& block)
	{
		INT sizeofNALU = 0;
		switch (m_lengthSizeMinusOne)
		{
		case 4:
		{
			sizeofNALU = ToINT32(m_naluPtr);
			m_naluPtr += 4;
			m_naluOffset += 4;
			block.video.size = sizeofNALU + 4;
			block.video.data = new BYTE[block.video.size];
			memcpy(block.video.data, &H264StartCode, 4);
			memcpy(block.video.data + 4, m_naluPtr, sizeofNALU);
			block.video.codeID = video->codeID;
			block.video.codeType = video->codeType;
			block.video.packetType = FLV_NALU;
			block.dts = m_dts;
			block.pts = block.dts + block.video.cts;
			m_naluPtr += sizeofNALU;
			m_naluOffset += sizeofNALU;
		}
		break;
		case 3:
		{
			sizeofNALU = ToINT24(m_naluPtr);
			m_naluPtr += 3;
			m_naluOffset += 3;
			block.video.size = sizeofNALU + 4;
			block.video.data = new BYTE[block.video.size];
			memcpy(block.video.data, &H264StartCode, 4);
			memcpy(block.video.data + 4, m_naluPtr, sizeofNALU);
			block.video.codeID = video->codeID;
			block.video.codeType = video->codeType;
			block.video.packetType = FLV_NALU;
			block.dts = m_dts;
			block.pts = block.dts + block.video.cts;
			m_naluPtr += sizeofNALU;
			m_naluOffset += sizeofNALU;
		}
		break;
		case 2:
		{
			sizeofNALU = ToINT16(m_naluPtr);
			m_naluPtr += 2;
			m_naluOffset += 2;
			block.video.size = sizeofNALU + 4;
			block.video.data = new BYTE[block.video.size];
			memcpy(block.video.data, &H264StartCode, 4);
			memcpy(block.video.data + 4, m_naluPtr, sizeofNALU);
			block.video.codeID = video->codeID;
			block.video.codeType = video->codeType;
			block.video.packetType = FLV_NALU;
			block.dts = m_dts;
			block.pts = block.dts + block.video.cts;
			m_naluPtr += sizeofNALU;
			m_naluOffset += sizeofNALU;
		}
		break;
		case 1:
		{
			sizeofNALU = ToINT8(m_naluPtr);
			m_naluPtr += 1;
			m_naluOffset += 1;
			block.video.size = sizeofNALU + 4;
			block.video.data = new BYTE[block.video.size];
			memcpy(block.video.data, &H264StartCode, 4);
			memcpy(block.video.data + 4, m_naluPtr, sizeofNALU);
			block.video.codeID = video->codeID;
			block.video.codeType = video->codeType;
			block.video.packetType = FLV_NALU;
			block.dts = m_dts;
			block.pts = block.dts + block.video.cts;
			m_naluPtr += sizeofNALU;
			m_naluOffset += sizeofNALU;
		}
		break;
		}
		if (m_naluOffset >= m_nalus.GetSize())
		{
			m_nalus.Clear();
		}
		return TRUE;
	}
	BOOL FLVReader::ParseScript(BYTE* data, INT size, FLV_SCRIPTDATA& script)
	{
		AMFObject metaObj;
		if (AMF_Decode(&metaObj, reinterpret_cast<CHAR*>(data), size, FALSE) > 0)
		{
			for (INT i = 0; i < metaObj.o_num; i++)
			{
				string val;
				AMFObjectProperty* prop = &metaObj.o_props[i];
				if (prop->p_type == AMF_OBJECT || prop->p_type == AMF_ECMA_ARRAY)
				{
					for (INT j = 0; j < prop->p_vu.p_object.o_num; j++)
					{
						AMFObjectProperty * objProp = &prop->p_vu.p_object.o_props[j];
						if (objProp->p_name.av_len > 0)
						{
							val.resize(objProp->p_name.av_len);
							val = objProp->p_name.av_val;
							if ("width" == val)
							{
								ASSERT(objProp->p_type == AMF_NUMBER);
								script.width = objProp->p_vu.p_number;
							}
							if ("height" == val)
							{
								ASSERT(objProp->p_type == AMF_NUMBER);
								script.height = objProp->p_vu.p_number;
							}
							if ("duration" == val)
							{
								ASSERT(objProp->p_type == AMF_NUMBER);
								script.duration = objProp->p_vu.p_number * 1000000;
							}
							if ("framerate" == val)
							{
								ASSERT(objProp->p_type == AMF_NUMBER);
								script.framerate = objProp->p_vu.p_number;
							}
							if ("filesize" == val)
							{
								ASSERT(objProp->p_type == AMF_NUMBER);
								script.filesize = objProp->p_vu.p_number;
							}
							if ("datasize" == val)
							{
								ASSERT(objProp->p_type == AMF_NUMBER);
								script.datasize = objProp->p_vu.p_number;
							}
							if ("audiosize" == val)
							{
								ASSERT(objProp->p_type == AMF_NUMBER);
								script.audiosize = objProp->p_vu.p_number;
							}
							if ("videosize" == val)
							{
								ASSERT(objProp->p_type == AMF_NUMBER);
								script.videosize = objProp->p_vu.p_number;
							}
							if ("videocodecid" == val)
							{
								ASSERT(objProp->p_type == AMF_NUMBER);
								script.videocodecid = objProp->p_vu.p_number;
							}
							if ("videodatarate" == val)
							{
								ASSERT(objProp->p_type == AMF_NUMBER);
								script.videodatarate = objProp->p_vu.p_number;
							}
							if ("audiocodecid" == val)
							{
								ASSERT(objProp->p_type == AMF_NUMBER);
								script.audiocodecid = objProp->p_vu.p_number;
							}
							if ("audiodatarate" == val)
							{
								ASSERT(objProp->p_type == AMF_NUMBER);
								script.audiodatarate = objProp->p_vu.p_number;
							}
							if ("audiosamplesize" == val)
							{
								ASSERT(objProp->p_type == AMF_NUMBER);
								script.audiosamplesize = objProp->p_vu.p_number;
							}
							if ("audiosamplerate" == val)
							{
								ASSERT(objProp->p_type == AMF_NUMBER);
								script.audiosamplerate = objProp->p_vu.p_number;
							}
						}
					}
				}
			}
			AMF_Reset(&metaObj);
			return TRUE;
		}
		return FALSE;
	}
	BOOL FLVReader::Close()
	{
		m_stream.Release();
		return TRUE;
	}
}
