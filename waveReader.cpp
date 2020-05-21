#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#define VERSION "1.0.0"

#define FORMATADDRESS "http://soundfile.sapp.org/doc/WaveFormat/"
using namespace std;
ifstream *getWaveFile(int argc, char* argv[]) {
    if (argc != 2) {
        string execName = string(argv[0]);
        cout << "error: argument number error." << endl << endl;
        cout << "to use waveReader, run '" << execName.substr(execName.rfind('\\') + 1, execName.length()) << " --help'" << "for more information." << endl;
        exit(-1);
    }
    else if (string(argv[1]).compare("--help") == 0 || string(argv[1]).compare("-h") == 0) {
        cout << "waveReader <file> | <-h> | <--help> | <-f> | <--format> " << endl;
        exit(-1);
    }
    else if (string(argv[1]).compare("--version") == 0 || string(argv[1]).compare("-v") == 0) {
        cout << "Welcome to use waveReader!" << endl;
        cout << "waveReader version: " << VERSION << endl;
        exit(-1);
    }
    else if (string(argv[1]).compare("--format") == 0 || string(argv[1]).compare("-f") == 0) {
        cout << "for more format information, visit this website " << FORMATADDRESS << endl;
        exit(1);
    }
    string filePath = string(argv[1]);
    ifstream *waveFile = new ifstream;
    waveFile->open(filePath.c_str(), ios::in | ios::binary);
    if (!waveFile->is_open()) {
        return NULL;
    }
    return waveFile;
}

int main(int argc, char* argv[]) // Executable file name , NULL
{
    ifstream *waveFile = getWaveFile(argc, argv);
    if (waveFile == NULL) {
        cout << "error: can't open file: " << argv[1] << endl;
    }

    unsigned char buf[5];
    string        ChunkID;
    unsigned int  ChunkSize = 0;
    string        Format;
    string        SubChunk1ID;
    unsigned int  SubChunk1Size = 0;
    unsigned int  AudioFormat = 0;
    unsigned int  NumberChannels = 0;
    unsigned int  SampleRate = 0;
    unsigned int  ByteRate = 0;
    unsigned int  BlockAlign = 0;
    unsigned int  BitPerSample = 0;
    string        SubChunk2ID;
    unsigned int  SubChunk2Size = 0;


    /* get ChunkId */
    for (int i = 0; i < 4; i++) {
        buf[i] = waveFile->get();
    }
    buf[4] = '\0';
    ChunkID = string((char*)buf);

    /* get ChunkSize */
    for (int i = 0; i < 4; i++) {
        ChunkSize += waveFile->get() << i*8;
    }

    /* get Format */
    for (int i = 0; i < 4; i++) {
        buf[i] = waveFile->get();
    }
    buf[4] = '\0';
    Format = string((char*)buf);

    /* get SubChunk1Id */
    for (int i = 0; i < 4; i++) {
        buf[i] = waveFile->get();
    }
    buf[4] = '\0';
    SubChunk1ID = string((char*)buf);

    /* get SubChunk1Size */
    for (int i = 0; i < 4; i++) {
        SubChunk1Size += waveFile->get() << i * 8;
    }

    /* get AudioFormat */
    for (int i = 0; i < 2; i++) {
        AudioFormat += waveFile->get() << i * 8;
    }

    /* get NuberChannels */
    for (int i = 0; i < 2; i++) {
        NumberChannels += waveFile->get() << i * 8;
    }

    /* get SampleRate */
    for (int i = 0; i < 4; i++) {
        SampleRate += waveFile->get() << i * 8;
    }

    /* get ByteRate */
    for (int i = 0; i < 4; i++) {
        ByteRate += waveFile->get() << i * 8;
    }

    /* get BlockAlign */
    for (int i = 0; i < 2; i++) {
        BlockAlign += waveFile->get() << i * 8;
    }

    /* get BitPerSample */
    for (int i = 0; i < 2; i++) {
        BitPerSample += waveFile->get() << i * 8;
    }

    /* get SubChunk2ID */
    for (int i = 0; i < 4; i++) {
        buf[i] = waveFile->get();
    }
    buf[4] = '\0';
    SubChunk2ID = string((char*)buf);

    /* get SubChunk2Size */
    for (int i = 0; i < 4; i++) {
        SubChunk2Size += waveFile->get() << i * 8;
    }
    cout 
         << ".wav resource file: " << argv[1]        << endl
         << ".wav format header: "                   << endl
         << "ChunkID             " << ChunkID        << endl
         << "ChunkSize           " << ChunkSize      << endl
         << "Format              " << Format         << endl
         << "SubChunk1ID         " << SubChunk1ID    << endl
         << "SubChunk1Size       " << SubChunk1Size  << endl
         << "AudioFormat         " << AudioFormat    << endl
         << "NumberChannels      " << NumberChannels << endl
         << "SampleRate          " << SampleRate     << endl
         << "ByteRate            " << ByteRate       << endl
         << "BlockAlign          " << BlockAlign     << endl
         << "BitPerSample        " << BitPerSample   << endl
         << "SubChunk2ID         " << SubChunk2ID    << endl
         << "SubChunk2Size       " << SubChunk2Size  << endl
         << endl;
    if (ChunkID.compare("RIFF")) {
        cout << "error: this is not '.wav' format.";
        exit(0);
    }
    if (AudioFormat != 1) {
        exit(0);
    }
    unsigned char sample8 = 0;
    short sample16 = 0;
    int NumberOfOut = 0;
#define DATAPERLINE 8                          
#define OUTLINENUM  10                         
#define DATAPEROUT  (DATAPERLINE*OUTLINENUM)   

    streampos cur = waveFile->tellg();
    waveFile->seekg(0,ios::end);
    unsigned int fileLen = (unsigned int)waveFile->tellg();
    waveFile->seekg(cur);

    cout << ".wav format data:" << endl;
    cout << "using Decimal/Hexadecimal number while BitPerSample equals 16/8" << endl;
    cout << endl;
    /* get data */
    while (!waveFile->eof()) {
        if (NumberChannels == 1) {
            /* channels == 1 bitPerSample == 8 */
            if (BitPerSample == 8) {
                sample8 = waveFile->get();
                NumberOfOut++;
                printf("%.2x ",sample8);
                if (NumberOfOut % DATAPERLINE == 0) {
                    cout << endl;
                }
                if (NumberOfOut % DATAPEROUT == 0) {
                    streampos pos = waveFile->tellg();
                    cout << "<" <<fileLen << "/" << pos << "> ";
                    cout << "press 'enter' for more sample data, 'ctrl+C' for exit." << endl;
                    getchar();
                }
            }
            /* channels == 1 bitPerSample == 16 */
            else if (BitPerSample == 16) {
                for (int i = 0; i < 2; i++) {
                    sample16 += waveFile->get() << i*8;
                }
                NumberOfOut += 2;
                printf("%6.5x ", sample16);
                sample16 = 0;
                if ((NumberOfOut / 2) % DATAPERLINE == 0) {
                    cout << endl;
                }
                if ((NumberOfOut / 2) % DATAPEROUT == 0) {
                    streampos pos = waveFile->tellg();
                    cout << "<" << fileLen << "/" << pos << "> ";
                    cout << "press 'enter' for more sample data, 'ctrl+C' for exit." << endl;
                    getchar();
                }
            }
        }
        else if (NumberChannels == 2) {
            /* channels == 2 bitPerSample == 8*/
            if (BitPerSample == 8) {

                /* left channel */
                sample8 = waveFile->get();
                NumberOfOut++;            //   L|R      L|R      L|R      L|R
                printf("%.2x ", sample8); // ff | ff  ff | ff  ff | ff  ff | ff 

                cout << "| ";

                /* right channel */
                sample8 = waveFile->get();
                NumberOfOut++;
                printf("%.2x ", sample8);

                if (NumberOfOut % DATAPERLINE == 0) {
                    cout << endl;
                }
                if (NumberOfOut % DATAPEROUT == 0) {
                    streampos pos = waveFile->tellg();
                    cout << "  L|R      L|R      L|R      L|R";
                    cout << "<" << fileLen << "/" << pos << "> ";
                    cout << "press 'enter' for more sample data, 'ctrl+C' for exit." << endl;
                    getchar();
                }
            }
            /* channels == 2 bitPerSample == 16*/
            else if (BitPerSample == 16) {
                /* left channel */
                for (int i = 0; i < 2; i++) {
                    sample16 += waveFile->get() << i * 8;
                } 
                NumberOfOut += 2;
                printf("%6.5d ", sample16);
                sample16 = 0;
                cout << "| ";
                /* right channel */
                for (int i = 0; i < 2; i++) {
                        sample16 += waveFile->get() << i * 8;
                }
                NumberOfOut += 2;
                printf("%6.5d ", sample16);
                sample16 = 0;
                
                if ((NumberOfOut / 2) % DATAPERLINE == 0) {
                    cout << endl;
                }
                if ((NumberOfOut / 2) % DATAPEROUT == 0) {
                    streampos pos = waveFile->tellg();
                    cout << "     L | R           L | R           L | R           L | R     " << endl;
                    cout << "<" << fileLen << "/" << pos << "> ";
                    cout << "press 'enter' for more sample data, 'ctrl+C' for exit." << endl;
                    getchar();
                }
            }
        }
        else {
            cout << "this application can not support " << NumberChannels << "channels." << endl;
            exit(0);
        }
    }
    cout << "end of wave file." << endl;
    waveFile->close();
    return 0;
}
