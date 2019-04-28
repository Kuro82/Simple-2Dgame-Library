/*
This library only supports csv for MacOX because of Linefeed code.
There is many bugs.
*/

#ifndef CSV_LOADER
#define CSV_LOADER

#include<vector>
#include<string>
#include<fstream>
#include<iostream>
#include<sstream>


class Csv{
        int rowSize, colSize;
        std::vector<std::vector<std::string> > dataBase;

        std::vector<std::string> split(std::string str){
            std::stringstream ss(str+",");
            std::vector<std::string> splitDatas;
            std::string tmpData;
            while( std::getline(ss, tmpData, ',') ){
                splitDatas.push_back(tmpData);
            }
            return splitDatas;
        }
        std::string delSpace(std::string str){
            str.erase(remove(str.begin(), str.end(), ' '), str.end());
            return str;
        }
    public:
        void resetData(void){
            rowSize = colSize = 0;
            dataBase.clear();
        }
        Csv(){ resetData(); }
        Csv(std::string filePath){
            resetData();
            std::ifstream ifs(filePath);
            if( ifs.fail() ){
                std::cerr<<"Error : csv file not found : "<<filePath<<std::endl;
            }else{
                std::string readRowDatas;
                while( std::getline(ifs, readRowDatas) ){
                    dataBase.push_back( split(readRowDatas) );
                    rowSize++;
                }
                if( dataBase.size() > 0 ){
                    colSize = dataBase[0].size();
                }
            }
        }
        int row(){ return rowSize; }
        int col(){ return colSize; }
        std::string data(int row, int col){
            if( 0<=row && row<rowSize && 0<=col && col<colSize ){
                return dataBase[row][col];
            }else{
                return "X";
            }
        }
        std::vector<std::vector<std::string> > getDataBase(){
            return dataBase;
        }
        void updateDataBase(std::vector<std::vector<std::string> > datas){
            resetData();
            dataBase = datas;
            rowSize = datas.size();
            colSize = datas[0].size();
        }
        void write(std::string fileName){
            std::ofstream ofs(fileName);
            for(int r=0;r<rowSize;r++){
                for(int c=0;c<colSize-1;c++){
                    ofs<<delSpace( data(r,c) )<<',';
                }
                ofs<<delSpace( data(r, colSize-1) );
                if( r<rowSize-1 ){
                    ofs<<std::endl;
                }
            }
            ofs.close();
        }
};

#endif