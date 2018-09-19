#ifndef _EASY_NET_TCP_WRITE_BUFFER_H
#define _EASY_NET_TCP_WRITE_BUFFER_H

namespace easy {
	namespace net {
		namespace tcp {
			class WriteBuffer
			{
			public:
				WriteBuffer(const unsigned char* data = nullptr, size_t length = 0);
				~WriteBuffer();

				void Reset(const unsigned char* data, size_t length);

				//返回没有写入的数据的首地址
				const unsigned char* Data();

				const unsigned char* SrcData();

				//返回剩余没有写入的数据的字节数
				size_t Length();

				size_t SrcLength();

			private:
				const unsigned char* m_pData;
				size_t m_length;

				//已经写入的数据的字节数
				size_t m_wLength;				
			};
		}
	}
}
#endif _EASY_NET_TCP_WRITE_BUFFER_H
