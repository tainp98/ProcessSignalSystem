#ifndef PACKAGEENCAPSULATION_H
#define PACKAGEENCAPSULATION_H
#include <vector>
#include <memory>
#include <memory.h>
#include <Message.h>
class PackageEncapsulation{
public:
    PackageEncapsulation();
    std::vector<char> toBytes(std::unique_ptr<Message> msg);
    std::unique_ptr<Message> toMessage(const std::vector<char>& data);

    template<typename T>
    std::vector<char> toBytes(T value){
        Type2Byte<T> convertData;
        std::vector<char> res(sizeof (T));
        convertData.value = value;
        if(!is_big_endian()){
            for(int i = 0; i < res.size(); i++){
                res[i] = convertData.data[res.size()-i-1];
            }
        }
        else{
            for(int i = 0; i < res.size(); i++){
                res[i] = convertData.data[i];
            }
        }
        return res;
    }
    template<typename T>
    T toValue(const std::vector<char>& data){
        if(sizeof (T) != data.size()) return T();
        Type2Byte<T> convertData;
        if(!is_big_endian()){
            for(int i = 0; i < data.size(); i++){
                convertData.data[i] = data[data.size()-i-1];
            }
        }
        else{
            for(int i = 0; i < data.size(); i++){
                convertData.data[i] = data[i];
            }
        }
        return convertData.value;
    }
private:
    char gencrc(char *data, size_t len);
protected:
    std::vector<char> mAccumulatingVector;
private:
    const char HEADER1 = 0xFF;
    const char HEADER2 = 0xAA;
    const int HEADER_LEN = 2;
    const int LENGTH_LEN = 1;
    const int SEQ_NUM_LEN = 4;
    const int ACK_NUM_LEN = 2;
    const int SENSORTYPE_LEN = 1;
    const int CHECKSUM_LEN = 1;
    const int TOTAL_SIZE_NO_PAYLOAD = 11;
};

#endif // PACKAGEENCAPSULATION_H
