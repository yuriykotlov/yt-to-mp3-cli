#include <iostream>
#include <limits>

enum AudioBitrate {
    NONE = 0,
    LOW = 128,
    MEDIUM = 192,
    HIGH = 256
};

std::string askForUrl() {
    std::string arg;

    std::cout << "enter url" << std::endl;
    std::cin >> arg;

    return arg;
}

AudioBitrate askForQuality() {
    int qualityIndex;
    AudioBitrate bitrateLevel = NONE;

    while (bitrateLevel == NONE) {
        std::cout << "enter quality of audio:\n\t0 = 128kbps\n\t1 = 192kbps\n\t2 = 256kbps" << std::endl;
        std::cin >> qualityIndex;

        if (std::cin.fail()) {
            // reset
            std::cin.clear();
            // clear line entered
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "quality entered is invalid" << std::endl;
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

    return bitrateLevel;
}

bool isUrlValid(const std::string& url) {
    const bool check1 = url.starts_with("https://www.youtube.com/");
    const bool check2 = url.starts_with("https://youtu.be/");

    return (check1 || check2);
}

void toMp3(const std::string& url, const AudioBitrate& bitrate) {
    const std::string finalCommand = "yt-dlp --extract-audio --audio-format mp3 --audio-quality "
                                    + std::to_string(bitrate) + "K"
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

    std::cout << std::endl;

    toMp3(url, askForQuality());

    // press any button to continue
    std::system("pause");

    return 0;
}
