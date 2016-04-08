#include <iostream>
#include <string>
#include <malloc.h>
#include <spibus.h>
#include <st_accel_dsh.h>

#define SPI_FILENAME "/dev/spidev1.0"

using namespace std;

bool SpiGetCmd(SpiBus & spibus, string arg){

    if (arg == "mode"){
        spi_mode spiarg;
        if (spibus.GetMode(spiarg))
            cout << "Mode " << (unsigned int)spiarg << " ("<< GetSpiModeDescription(spiarg) << ")" << endl;
        else
            cout << "An error occured!" << endl;
    } else if (arg == "speed"){
        hertz spiarg;
        if (spibus.GetSpeed(spiarg))
            cout << spiarg << " Hz" << endl;
        else
            cout << "An error occured!" << endl;
    } else if (arg == "bpw") {
        bitsperword spiarg;
        if (spibus.GetBitsPerWord(spiarg))
            cout << (unsigned int)spiarg << " bits per word" << endl;
        else
            cout << "An error occured!" << endl;
    } else if (arg == "endian") {
        endianess spiarg;
        if (spibus.GetEndianness(spiarg))
            cout << ((bool)spiarg?"LSB":"MSB") << endl;
        else
            cout << "An error occured!" << endl;
    } else if (arg == "delay") {
        microseconds spiarg;
        if (spibus.GetDelay(spiarg))
            cout << spiarg << " us" << endl;
        else
            cout << "An error occured!" << endl;
    } else
        return false;
    return true;
}
bool SpiSetCmd(SpiBus & spibus, string arg){
    string subarg;
    if (arg.substr(0,4) == "mode"){
        spi_mode spiarg;
        subarg = arg.erase(0,5);

        try {
            unsigned int tmp = stoul(subarg, NULL);
            spiarg = (spi_mode)tmp;
        } catch (...) {
            return false;
        }

        if (spibus.SetMode(spiarg))
            cout << "Mode " << (unsigned int)spiarg << " ("<< GetSpiModeDescription(spiarg) << ")" << endl;
        else
            cout << "An error occured!" << endl;
    } else if (arg.substr(0,5) == "speed"){
        hertz spiarg;
        subarg = arg.erase(0,6);

        try {
            spiarg = stoul(subarg, NULL);
        } catch (...) {
            return false;
        }

        if (spibus.SetSpeed(spiarg))
            cout << spiarg << " Hz" << endl;
        else
            cout << "An error occured!" << endl;
    } else if (arg.substr(0,3) == "bpw") {
        bitsperword spiarg;
        subarg = arg.erase(0,4);

        try {
            unsigned int tmp = stoul(subarg, NULL);
            spiarg = (bitsperword)tmp;
        } catch (...) {
            return false;
        }

        if (spibus.SetBitsPerWord(spiarg))
            cout << (unsigned int)spiarg << " bits per word" << endl;
        else
            cout << "An error occured!" << endl;
    } else if (arg.substr(0,6) == "endian") {
        endianess spiarg;
        subarg = arg.erase(0,7);

        try {
            unsigned int tmp = stoul(subarg, NULL);
            spiarg = (tmp==0 ? MSB : LSB );
        } catch (...) {
            return false;
        }

        if (spibus.SetEndianness(spiarg))
            cout << ((bool)spiarg?"LSB":"MSB") << endl;
        else
            cout << "An error occured!" << endl;
    } else if (arg.substr(0,5) == "delay") {
        microseconds spiarg;
        subarg = arg.erase(0,6);

        try {
            spiarg = stoul(subarg, NULL);
        } catch (...) {
            return false;
        }

        if (spibus.SetSpeed(spiarg))
            cout << spiarg << " Hz" << endl;
        else
            cout << "An error occured!" << endl;
    } else
        return false;
    return true;
}
void PrintHelp(){
    cout << "--- Spi Test Tool ---" << endl << endl;
    cout << "Commands:" << endl;
    cout << "\tread\tReads whatever valid argument is passed from the spi device" << endl;
    cout << "\twrite\tAttemts to write a valid argument to the spi device" << endl;
    cout << "\test\tWrites to the SPI device and recieves the same number of byts written" << endl;
    cout << "\thelp\tShow the help message" << endl;
    cout << "\tquit\tQuits the program" << endl << endl;
    cout << "Arguments:" << endl;
    cout << "\tmode (0-3), speed (Hz), bpw (bits), delay (us), endian (1 = LSB, 0 = MSB)" << endl << endl;
}
int StartSpiTester(){
    string cmd;
    SpiBus spi0(SPI_FILENAME);

    PrintHelp();

    if(spi0.Open()){
        while(spi0.IsOpen()){
            cout << "> ";
            getline (std::cin,cmd);

            if(cmd == "quit" || cmd == "exit"){
                spi0.Close();
            }else if(cmd == "help"){
                PrintHelp();
            }else if(cmd.substr(0,4) == "read"){
                string arg = cmd.erase(0,5);
                if (!SpiGetCmd(spi0,arg))
                    cout << "Invalid arguement" << endl;
            }else if(cmd.substr(0,5) == "write"){
                string arg = cmd.erase(0,6);
                if (!SpiSetCmd(spi0,arg))
                    cout << "Invalid arguement" << endl;
            }else if(cmd.substr(0,4) == "test"){

            }
        }
    }else{
        cout << "could not open spi device" << endl;
    }

    return 0;
}

int BasicAccelerometerTest(){
    try{
        StAccel_dsh * accel = new StAccel_dsh();
        // do stuff with acceleromter
        delete accel;
    }catch(int e){
        cout << e << endl;
        return 1;
    }
    return 0;
}

int main(){
    return BasicAccelerometerTest();
}
