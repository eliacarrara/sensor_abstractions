#include <iostream>
#include <string>
#include <malloc.h>
#include <spibus.h>
#include <st_accel_dsh.h>

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
void PrintSpiHelp(){
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
int SpiTester(){
    string cmd;
    SpiBus spi0("/dev/spidev1.0");

    PrintSpiHelp();

    if(spi0.Open()){
        while(spi0.IsOpen()){
            cout << "> ";
            getline (std::cin,cmd);

            if(cmd == "quit" || cmd == "exit"){
                spi0.Close();
            }else if(cmd == "help"){
                PrintSpiHelp();
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

void PrintAccelHelp(){
    cout << "--- ST LIS3DSH Accelerometer Test Tool---" << endl << endl;
    cout << "Commands:" << endl;
    cout << "\thelp\tShow the help message" << endl;
    cout << "\tquit\tQuits the program" << endl;
    cout << "\tcall\tcalls a method of the object" << endl;
}

int AccelerometerTest(){
    string cmd;
    StAccel_dsh * accel = NULL;
    bool loop = true;

    PrintAccelHelp();

    try {
        accel = new StAccel_dsh();
    } catch (int e) {
        cout << "Exception: " << e << endl;
        if(accel != NULL)
            delete accel;
        return 1;
    }

    while(loop){
        cout << "> ";
        getline (std::cin, cmd);

        if(cmd == "quit" || cmd == "exit"){
            loop = false;
            break;
        }
        else if(cmd == "help"){
            PrintAccelHelp();
        }else if(cmd.substr(0,9) == "SoftReset" || cmd.substr(0,9) == "softreset"){
            cout << (accel->SoftReset() ? "o " : "x " );
        }else  if(cmd.substr(0,6) == "Reboot" || cmd.substr(0,6) == "reboot"){
            cout << (accel->Reboot() ? "o " : "x " );
        }else if(cmd.substr(0,8) == "ReadInfo" || cmd.substr(0,8) == "readinfo"){
            unsigned short val;
            bool rtrn = accel->ReadInfomation(val);
            cout << "\t" << val << endl;
            cout << (rtrn ? "o " : "x " );
        }else if(cmd.substr(0,6) == "WhoAmI" || cmd.substr(0,6) == "whoami"){
            char val;
            bool rtrn = accel->WhoAmI(val);
            cout << "\t" << (unsigned int)val << endl;
            cout << (rtrn ? "o " : "x " );
        }else if(cmd.substr(0,6) == "GetODR" || cmd.substr(0,6) == "getodr"){
            StAccel_dsh::ODR val;
            bool rtrn = accel->GetODR(val);
            cout << "\t" << (unsigned int)val << endl;
            cout << (rtrn ? "o " : "x " );
        }else if(cmd.substr(0,6) == "SetODR" || cmd.substr(0,6) == "setodr"){
            char val;

            try {
                val = (char)stoul(cmd.erase(0,7), NULL);
            } catch (...) {
                cout << "x ";
                continue;
            }

            cout << (accel->SetODR((StAccel_dsh::ODR)val) ? "o " : "x " );
        }else if(cmd.substr(0,9) == "GetRange" || cmd.substr(0,9) == "getrange"){
            StAccel_dsh::MeasureRange val;
            bool rtrn = accel->GetRange(val);
            cout << "\t" << (unsigned int)val << endl;
            cout << (rtrn ? "o " : "x " );
        }else if(cmd.substr(0,8) == "SetRange" || cmd.substr(0,8) == "setrange"){
            char val;

            try {
                val = (char)stoul(cmd.erase(0,9), NULL);
            } catch (...) {
                cout << "x ";
                continue;
            }

            cout << (accel->SetRange((StAccel_dsh::MeasureRange)val) ? "o " : "x " );
        }else if(cmd.substr(0,13) == "IsFifoEnabled" || cmd.substr(0,13) == "isfifoenabled"){
            bool val;
            bool rtrn = accel->IsFifoEnabled(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }else if(cmd.substr(0,6) == "UseFifo" || cmd.substr(0,6) == "usefifo"){
            char val;

            try {
                val = (char)stoul(cmd.erase(0,7), NULL);
            } catch (...) {
                cout << "x ";
                continue;
            }

            cout << (accel->UseFifo((val==0?false:true)) ? "o " : "x " );
        }else if(cmd.substr(0,14) == "DataOverrunXYZ" || cmd.substr(0,14) == "dataoverrunxyz"){
            bool val;
            bool rtrn = accel->DataOverrunXYZ(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }else if(cmd.substr(0,12) == "DataOverrunX" || cmd.substr(0,12) == "dataoverrunx"){
            bool val;
            bool rtrn = accel->DataOverrunX(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }else if(cmd.substr(0,12) == "DataOverrunY" || cmd.substr(0,12) == "dataoverruny"){
            bool val;
            bool rtrn = accel->DataOverrunY(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }else if(cmd.substr(0,12) == "DataOverrunZ" || cmd.substr(0,12) == "dataoverrunz"){
            bool val;
            bool rtrn = accel->DataOverrunZ(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }else if(cmd.substr(0,16) == "DataAvailableXYZ" || cmd.substr(0,16) == "dataavailablexyz"){
            bool val;
            bool rtrn = accel->DataAvailableXYZ(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }else if(cmd.substr(0,14) == "DataAvailableX" || cmd.substr(0,14) == "dataavailablex"){
            bool val;
            bool rtrn = accel->DataAvailableX(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }else if(cmd.substr(0,14) == "DataAvailableY" || cmd.substr(0,14) == "dataavailabley"){
            bool val;
            bool rtrn = accel->DataAvailableY(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }else if(cmd.substr(0,14) == "DataAvailableZ" || cmd.substr(0,14) == "dataavailablez"){
            bool val;
            bool rtrn = accel->DataAvailableZ(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }else if(cmd.substr(0,14) == "IsXAxisEnabled" || cmd.substr(0,14) == "isxaxisenabled"){
            bool val;
            bool rtrn = accel->IsXAxisEnabled(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }else if(cmd.substr(0,14) == "IsYAxisEnabled" || cmd.substr(0,14) == "isyaxisenabled"){
            bool val;
            bool rtrn = accel->IsYAxisEnabled(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }else if(cmd.substr(0,14) == "IsZAxisEnabled" || cmd.substr(0,14) == "iszaxisenabled"){
            bool val;
            bool rtrn = accel->IsZAxisEnabled(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }else if(cmd.substr(0,8) == "UseAxisX" || cmd.substr(0,8) == "useaxisx"){
            char val;

            try {
                val = (char)stoul(cmd.erase(0,9), NULL);
            } catch (...) {
                cout << "x ";
                continue;
            }

            cout << (accel->UseAxisX((val==0?false:true)) ? "o " : "x " );
        }else if(cmd.substr(0,8) == "UseAxisY" || cmd.substr(0,8) == "useaxisy"){
            char val;

            try {
                val = (char)stoul(cmd.erase(0,9), NULL);
            } catch (...) {
                cout << "x ";
                continue;
            }

            cout << (accel->UseAxisY((val==0?false:true)) ? "o " : "x " );
        }else if(cmd.substr(0,8) == "UseAxisZ" || cmd.substr(0,8) == "useaxisz"){
            char val;

            try {
                val = (char)stoul(cmd.erase(0,9), NULL);
            } catch (...) {
                cout << "x ";
                continue;
            }

            cout << (accel->UseAxisZ((val==0?false:true)) ? "o " : "x " );
        }else if(cmd.substr(0,5) == "IsBSU" || cmd.substr(0,5) == "isbsu"){
            bool val;
            bool rtrn = accel->IsBSU(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }else if(cmd.substr(0,6) == "UseBSU" || cmd.substr(0,6) == "usebsu"){
            char val;

            try {
                val = (char)stoul(cmd.erase(0,7), NULL);
            } catch (...) {
                cout << "x ";
                continue;
            }

            cout << (accel->UseBSU((val==0?false:true)) ? "o " : "x " );
        }else if(cmd.substr(0,9) == "IsReadInc" || cmd.substr(0,9) == "isreadinc"){
            bool val;
            bool rtrn = accel->IsReadInc(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }else if(cmd.substr(0,10) == "UseReadInc" || cmd.substr(0,10) == "usereadinc"){
            char val;

            try {
                val = (char)stoul(cmd.erase(0,11), NULL);
            } catch (...) {
                cout << "x ";
                continue;
            }

            cout << (accel->UseReadInc((val==0?false:true)) ? "o " : "x " );
        }else if(cmd.substr(0,15) == "ReadAccel" || cmd.substr(0,15) == "readaccel"){

        }else if(cmd.substr(0,14) == "ReadTemp" || cmd.substr(0,14) == "readtemp"){
            Sensor::RawThermometerData val;
            bool rtrn = accel->ReadSensorDataOnce(val);
            cout << "\t" << (unsigned int)val.nTemp << endl;
            cout << (rtrn ? "o " : "x " );
        }else
            cout << "invalid command" << endl;
    }
    delete accel;
    return 0;
}

int main(){
    int selection;
    cout << "---Skywalker Prototype---" << endl;
    cout << "Select what to test:" << endl;
    cout << "\t[0] spi bus" << endl;
    cout << "\t[1] test ST LIS3DSH (Accelerometer)" << endl;
    cout << "> ";
    cin >> selection;

    switch (selection) {
    case 0:
        return SpiTester();
    case 1:
        return AccelerometerTest();
    default:
        cout << "Invalid Option!"<< endl;
        return 1;
    }

}
