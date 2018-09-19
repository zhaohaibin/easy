#include "WriteBuffer.h"

namespace easy {
	namespace net {
		namespace tcp {

			WriteBuffer::WriteBuffer(const unsigned char* data, size_t length) : m_pData(data)
				, m_length(length)
				, m_wLength(0)
			{

			}

			WriteBuffer::~WriteBuffer()
			{
				m_pData = nullptr;
				m_length = 0;
				m_wLength = 0;
			}

			void WriteBuffer::Reset(const unsigned char* data, size_t length)
			{
				m_pData = data;
				m_length = length;
				m_wLength = 0;
			}

			const unsigned char* WriteBuffer::Data()
			{
				return m_pData + m_wLength;
			}

			const unsigned char* WriteBuffer::SrcData()
			{
				return m_pData;
			}

			size_t WriteBuffer::Length()
			{
				return m_length - m_wLength;
			}

			size_t WriteBuffer::SrcLength()
			{
				return m_length;
			}

		}
	}
}