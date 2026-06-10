#include <iostream>
#include <limits>
#include <regex>

enum AudioBitrate {
    NONE = 0,
    LOW = 128,
    MEDIUM = 192,
    HIGH = 256
};

constexpr std::string_view illegalCharacters = "\\/:*?\"<>|";

std::string askForUrl() {
    std::string arg;

    std::cout << "enter url" << std::endl;
    std::getline(std::cin, arg);

    std::cout << std::endl;

    return arg;
}

AudioBitrate askForQuality() {
    int qualityIndex;
    AudioBitrate bitrateLevel = NONE;

    while (bitrateLevel == NONE) {
        std::cout << "enter quality of audio:\n\t0 = 128kbps\n\t1 = 192kbps\n\t2 = 256kbps" << std::endl;
        std::cin >> qualityIndex;
        std::cout << std::endl;

        if (std::cin.fail()) {
            // reset
            std::cin.clear();
            // clear line entered
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "quality entered is invalid" << std::endl;
            std::cout << std::endl;

            continue;
        }

        switch (qualityIndex) {
            case 0:
                bitrateLevel = LOW;
                break;
            case 1:
                bitrateLevel = MEDIUM;
                break;
            case 2:
                bitrateLevel = HIGH;
                break;
            default:
                std::cout << "quality entered is invalid" << std::endl;
                break;
        }
    }

    // cause of the std::cin for qualityIndex, clear the buffer so that there's no leftover text
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return bitrateLevel;
}

std::string askForFileName() {
    std::string createOutputFlag = "?";

    while (tolower(createOutputFlag[0]) != 'y' && tolower(createOutputFlag[0]) != 'n') {
        std::cout << "do you want to set the file name? (yes/no)" << std::endl;
        std::getline(std::cin, createOutputFlag);
        std::cout << std::endl;
    }

    if (tolower(createOutputFlag[0]) == 'y') {
        std::string fileName;
        std::cout << "enter a file name" << std::endl;
        std::cout << "(note) these characters are not allowed: \\ / : * ? \" < > |" << std::endl;
        std::getline(std::cin, fileName);

        std::cout << std::endl;

        // if there is an illegal character found
        while (fileName.empty() || fileName.find_first_of(illegalCharacters) != std::string::npos) {
            std::cout << "you have entered an illegal character, try enter a file name again" << std::endl;
            std::cout << "(note) these characters are not allowed: \\ / : * ? \" < > |" << std::endl;
            std::getline(std::cin, fileName);
            std::cout << std::endl;
        }

        // yt-dlp autofills the .%(ext)s part with the file type selected
        return "--output \"" + fileName + ".%(ext)s\"";
    }

    return "";
}

bool isUrlValid(const std::string& url) {
    const bool check1 = url.starts_with("https://www.youtube.com/");
    const bool check2 = url.starts_with("https://youtu.be/");

    return (check1 || check2);
}

void toMp3(const std::string& url, const AudioBitrate& bitrate, const std::string& fileNameFlag) {
    const std::string finalCommand = "yt-dlp --extract-audio --audio-format mp3 --audio-quality "
                                    + std::to_string(bitrate) + "K "
                                    + fileNameFlag
                                    + " \""
                                    + url
                                    + "\"";

    std::system(finalCommand.c_str());
}

int main(int argc, char* argv[]) {
    std::string url = argc > 1 ? argv[1] : askForUrl();

    // check if url is valid
    while (!isUrlValid(url)){
        std::cout << "url \"" + url + "\" is invalid" << std::endl;
        url = askForUrl();
    }

    toMp3(url, askForQuality(), askForFileName());

    // press any button to continue
    std::system("pause");

    return 0;
}
