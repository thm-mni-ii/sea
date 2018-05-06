//
// Created by jmeintrup on 02.05.18.
//

#ifndef SEA_LOCALDYCKTABLE_H
#define SEA_LOCALDYCKTABLE_H
namespace Sealib {
    class LocalDyckTable {

    public:
        class Data {
        public:
            unsigned char* localMatches;
            char* localDepths;
            unsigned char leftPioneer;
            unsigned char rightPioneer;
            Data(unsigned char len);
            virtual ~Data();
        };

        unsigned long getEntries() const;
        static Data* calculateLocalData(unsigned long g, unsigned char len);
        const Data* operator[](unsigned long i);
        virtual ~LocalDyckTable();

        explicit LocalDyckTable(unsigned char len);
        LocalDyckTable();

    private:
        Data **table;
        unsigned long entries;

    };
}
#endif //SEA_LOCALDYCKTABLE_H
