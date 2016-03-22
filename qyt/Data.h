//
// Created by noccolor on 16/3/19.
//

#ifndef OPENGL_DATA_H
#define OPENGL_DATA_H

#include <sys/_types/_ssize_t.h>

namespace qyt
{
    /**
     * Data
     * 是一种对二进制数据的封装
     */
    class Data
    {
    public:
        static const Data Null;

        Data();
        Data(const Data& other);
        Data(Data&& other);
        ~Data();

        Data& operator= (const Data& other);
        Data& operator= (Data&& other);

        unsigned char* getBytes() const;

        ssize_t getSize() const;

        void copy(const unsigned char* bytes, const ssize_t size);
        void fastSet(unsigned char* bytes, const ssize_t size);
        void clear();
        bool isNull() const;
    private:
        unsigned char* _bytes;
        ssize_t _size;
        void move(Data& other);
    };
}

#endif //OPENGL_DATA_H
