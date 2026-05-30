# 🎓 UniBot v5.0 - AI-Powered University Student Chatbot

## 📋 Table of Contents
1. [Overview](#overview)
2. [Features](#features)
3. [Requirements](#requirements)
4. [Installation](#installation)
5. [Configuration](#configuration)
6. [Usage](#usage)
7. [Commands](#commands)
8. [File Structure](#file-structure)
9. [Platform Support](#platform-support)
10. [Troubleshooting](#troubleshooting)
11. [Image Setup](#image-setup)
12. [API Functions](#api-functions)
13. [Advanced Features](#advanced-features)
14. [Examples](#examples)

---

## 📖 Overview

**UniBot v5.0** is a comprehensive C++ console-based chatbot designed specifically for helping new university students navigate campus life. The chatbot combines voice recognition, intelligent text processing, image display with resizing capabilities, and Google Maps integration to provide an interactive and engaging student assistance experience.

### Key Highlights:
- 🎤 **Speech Recognition** - Students can speak their questions naturally
- 🔊 **Voice Output** - Answers are read aloud automatically
- 🖼️ **Image Support** - Displays campus images in JPG/JFIF format
- 📐 **Smart Resizing** - Automatically scales images to optimal sizes
- 🗺️ **Google Maps** - Shows exact school location on interactive maps
- 💾 **Chat History** - Records and saves all conversations
- 📅 **Smart Context** - Understands date, time, and contextual information
- 🎲 **Dynamic Responses** - Multiple response variations for natural interaction

---

## ✨ Features

### Core Features
| Feature | Description |
|---------|-------------|
| **11+ Campus Topics** | Covers library, registration, advisors, WiFi, dining, etc. |
| **Voice Recognition** | Windows Speech API, macOS voice control, Linux espeak |
| **Voice Output** | Cross-platform text-to-speech synthesis |
| **Image Display** | Opens JPG/JFIF images in default viewer |
| **Image Resizing** | Dynamic scaling with ImageMagick/sips |
| **Google Maps** | Direct integration with school location |
| **Chat History** | Track all student interactions |
| **File Export** | Save conversations as text files |
| **Case-Insensitive** | Natural language processing |
| **Error Handling** | Graceful handling of missing files/resources |

### Advanced Features
- **Auto-Format Detection** - Recognizes both .jpg and .jfif files
- **Cached Resizing** - Stores resized images to avoid redundant processing
- **Dynamic Image Dimensions** - Change image size on-the-fly with `resize` command
- **Random Responses** - Varying acknowledgments and farewells
- **Smart Keyword Matching** - Detects intent from partial phrases
- **Session Summary** - Statistics on chatbot usage
- **Multi-Platform** - Windows, macOS, Linux support

---

## 📋 Requirements

### System Requirements
```
- C++17 or higher
- 100MB disk space (for images and scaled versions)
- Audio input device (microphone for voice input)
- Audio output device (speakers for voice output)
```

### Software Dependencies

#### Windows
- Microsoft Visual C++ Redistributable
- PowerShell (built-in Windows)
- ImageMagick (optional, for image resizing)
  Download: https://imagemagick.org/script/download.php#windows

#### macOS
- Xcode Command Line Tools
- sips (built-in, no installation needed)
- espeak or festival (optional)

#### Linux
- GCC/G++ compiler
- ImageMagick
- espeak or festival

---

## 💾 Installation

### Step 1: Get the Code
```bash
git clone https://github.com/sackeyelvin-cell/The-SACMOX-S-Barber.-Githhub.io.git
```

### Step 2: Create Directories
```bash
mkdir campus_images
mkdir scaled_images
```

### Step 3: Add Images
Place JPG/JFIF images in `campus_images/`:
- library.jpg / library.jfif
- registration.jpg / registration.jfif
- advisor.jpg / advisor.jfif
- wifi.jpg / wifi.jfif
- campus_map.jpg / campus_map.jfif
- dining.jpg / dining.jfif
- student_id.jpg / student_id.jfif
- financial_aid.jpg / financial_aid.jfif
- clubs.jpg / clubs.jfif
- tutoring.jpg / tutoring.jfif

### Step 4: Compile
```bash
g++ -std=c++17 -o unibot student_chatbot_with_maps.cpp
```

### Step 5: Run
```bash
./unibot
```

---

## ⚙️ Configuration

### Set Your School Location

Edit lines 92-96 in `student_chatbot_with_maps.cpp`:

```cpp
const double SCHOOL_LATITUDE = 40.8075;      // Your school's latitude
const double SCHOOL_LONGITUDE = -75.1973;    // Your school's longitude
const string SCHOOL_NAME = "University Campus";
const string SCHOOL_ADDRESS = "123 University Avenue";
```

### Find Coordinates via Google Maps

1. Go to https://www.google.com/maps
2. Search for your school
3. Right-click on location
4. Copy the coordinates

### Popular University Coordinates

```cpp
// University of Pennsylvania
40.8075, -75.1973

// Harvard University
42.3601, -71.1968

// MIT
42.3599, -71.0961

// Yale University
41.3109, -72.9268

// Stanford University
37.4275, -122.1697
```

---

## 🎮 Usage

### Start the Chatbot
```bash
./unibot
```

### Ask Questions
```
📝 You: Where is the library?

🖼️  Opening image: campus_images/library.jpg (Resized: 800x600)

🤖 UniBot: The main library is located in the Academic Building...
```

### Enable Voice
```
📝 You: voice
🤖 UniBot: Voice mode enabled.

🎤 You (speak or type): Where is the cafeteria?
[Bot speaks the answer aloud]
```

---

## 📝 Commands Reference

| Command | Purpose | Example |
|---------|---------|---------|
| `voice` | Toggle voice mode | `voice` |
| `history` | Show chat history | `history` |
| `save` | Export chat to file | `save` |
| `resize W H` | Change image size | `resize 1024 768` |
| `quit` / `exit` / `bye` | Exit chatbot | `quit` |

### Image Resize Presets

```
resize 400 300    - Thumbnail
resize 640 480    - Small (VGA)
resize 800 600    - Medium (DEFAULT)
resize 1024 768   - Large (XGA)
resize 1280 960   - HD
resize 1920 1440  - Full HD
```

---

## 📁 File Structure

```
project_folder/
├── student_chatbot_with_maps.cpp    # Main program
├── README.md                         # Documentation
├── chat_history.txt                 # Generated (save command)
├── campus_images/                   # Your images folder
│   ├── library.jpg
│   ├── registration.jpg
│   ├── advisor.jpg
│   ├── wifi.jpg
│   ├── campus_map.jpg
│   ├── dining.jpg
│   ├── student_id.jpg
│   ├── financial_aid.jpg
│   ├── clubs.jpg
│   └── tutoring.jpg
└── scaled_images/                   # Auto-generated resized images
```

---

## 🖥️ Platform Support

### Windows
```bash
g++ -std=c++17 -o unibot.exe student_chatbot_with_maps.cpp
unibot.exe
```

### macOS
```bash
xcode-select --install  # First time only
g++ -std=c++17 -o unibot student_chatbot_with_maps.cpp
./unibot
```

### Linux
```bash
sudo apt-get install g++ imagemagick espeak
g++ -std=c++17 -o unibot student_chatbot_with_maps.cpp
./unibot
```

---

## 🐛 Troubleshooting

### Images Not Displaying
**Solution:** Verify image paths and names match exactly:
```bash
ls -la campus_images/
# Names must be: library.jpg (not Library.jpg)
```

### Voice Not Working
**Windows:**
- Check microphone is connected and set as default
- Verify PowerShell: `powershell -v`

**macOS:**
- Grant microphone permissions in System Preferences
- Ensure speakers are connected

**Linux:**
- Install espeak: `sudo apt-get install espeak`
- Check audio devices: `arecord -l`

### Image Resizing Not Working
**Windows:**
- Install ImageMagick: https://imagemagick.org/script/download.php#windows

**macOS:**
- sips is built-in (should work automatically)

**Linux:**
- Install: `sudo apt-get install imagemagick`

### Google Maps Not Opening
**Solution:** Check your browser is set as default:
- Windows: Settings → Apps → Default apps
- macOS: System Preferences → General
- Linux: `xdg-settings set default-web-browser`

### Compilation Errors
**Solution:** Use C++17 flag:
```bash
g++ -std=c++17 -o unibot student_chatbot_with_maps.cpp
```

On older systems:
```bash
g++ -std=c++17 -lstdc++fs -o unibot student_chatbot_with_maps.cpp
```

---

## 🖼️ Image Setup

### Supported Formats
- ✅ JPG / JPEG
- ✅ JFIF
- ❌ PNG (not supported)
- ❌ BMP (not supported)

### Recommended Specs
- **Default Size:** 800x600 pixels
- **File Size:** < 2MB per image
- **Quality:** 75-85 (good balance)
- **Format:** JPG or JFIF

### Convert Images with ImageMagick
```bash
# Resize
convert input.png -resize 800x600 output.jpg

# Compress
convert input.jpg -quality 85 output.jpg

# PNG to JPG
convert input.png output.jpg
```

### Auto-Detection
The chatbot searches in this order:
```
1. library.jpg
2. library.JPG
3. library.jpeg
4. library.jfif
5. library.JFIF
```

---

## 🔧 API Functions

### Display Functions
```cpp
void displayImage(string path, bool resize=true, int w=800, int h=600);
void displayImages(vector<string> paths, bool resize=true, int w=800, int h=600);
void displayThumbnail(string path, int w=400, int h=300);
```

### Image Processing
```cpp
void resizeImage(string input, string output, int width, int height);
string getScaledImagePath(string path, int width=800, int height=600);
string findImageFile(string basePath);
```

### Maps
```cpp
void openGoogleMaps(double lat, double lon, string name);
```

### Voice
```cpp
void speak(string text);
string getSpeechInput();
```

### Chat
```cpp
void addToHistory(string question, string answer);
void displayChatHistory();
void saveChatHistory(string filename="chat_history.txt");
```

### Utilities
```cpp
string toLowerCase(string str);
string getRandomResponse(int type);
string getCurrentDateTime();
string getCurrentTime();
string getDayOfWeek();
```

---

## 🚀 Advanced Features

### Add New Topics

```cpp
else if (question.find("dormitory") != string::npos) {
    string imagePath = findImageFile("campus_images/housing");
    displayImage(imagePath, true, 800, 600);
    answer = "Our dormitories are located...";
}
```

### Custom Response Variations

```cpp
vector<string> responses = {
    "Custom response 1",
    "Custom response 2",
    "Custom response 3"
};
answer = responses[rand() % responses.size()];
```

### Different Image Sizes Per Topic

```cpp
if (question.find("library") != string::npos) {
    displayImage(imagePath, true, 1024, 768);  // Larger
} else if (question.find("wifi") != string::npos) {
    displayImage(imagePath, true, 640, 480);   // Smaller
}
```

---

## 📚 Examples

### Full Conversation
```
📝 You: Where is the school located?

📍 School Location Information:
   Name: University Campus
   Address: 123 University Avenue
   Coordinates: 40.8075, -75.1973

🖼️  Opening image: campus_images/campus_map.jpg
🗺️  Opening Google Maps...

🤖 UniBot: Our campus is located at 123 University Avenue. 
I've opened Google Maps showing our exact location.
```

### Voice Session
```
🎤 You: How do I register?
You said: How do I register?

🖼️  Opening image: campus_images/registration.jpg

🤖 UniBot: You can register for classes through the Student Portal...
[Bot speaks answer aloud]
```

### Image Resizing
```
📝 You: resize 1280 960

🤖 UniBot: Image resize set to 1280x960 pixels.

📝 You: What clubs are available?

🖼️  Opening image: campus_images/clubs.jpg (Resized: 1280x960)

🤖 UniBot: We have 150+ student clubs and organizations!
```

---

## FAQ

**Q: Can I add more topics?**  
A: Yes! Add new `else if` statements in `answerQuestion()`.

**Q: How to change default image size?**  
A: Edit line 441-442:
```cpp
int imageWidth = 800;
int imageHeight = 600;
```

**Q: Which image formats work?**  
A: JPG and JFIF. Convert others using ImageMagick.

**Q: How to change school location?**  
A: Edit lines 92-96 with your coordinates from Google Maps.

**Q: How to disable voice?**  
A: Type `voice` to toggle. Starts in text mode.

---

## 📞 Support

1. Check the Troubleshooting section
2. Verify file paths and permissions
3. Review example interactions
4. Check platform requirements
5. Read compiler error messages carefully

---

## 🔄 Future Enhancements

Potential features:
- 💬 NLP integration
- 🌐 Web interface
- 📲 Mobile app
- 🗃️ Database integration
- 🎨 GUI application
- 🔐 User authentication
- 📊 Analytics
- 🤖 AI/ML engine
- 🌍 Multi-language support

---

## 📄 License

Educational resource - free to modify and adapt.

---

## 👨‍💻 Author

**Sackey Elvin** (@sackeyelvin-cell)

Repository: https://github.com/sackeyelvin-cell/The-SACMOX-S-Barber.-Githhub.io

---

**Version:** 5.0 | **Status:** Production Ready ✅ | **Last Updated:** May 30, 2026

---

## Quick Reference

```
╔════════════════════════════════════════╗
║       UniBot v5.0 Quick Reference     ║
╠════════════════════════════════════════╣
║ COMPILE:  g++ -std=c++17 -o unibot *  ║
║ RUN:      ./unibot                    ║
║                                        ║
║ COMMANDS:                              ║
║  voice       - Toggle voice mode      ║
║  history     - Show chat history      ║
║  save        - Save to file           ║
║  resize W H  - Change image size      ║
║  quit        - Exit                   ║
║                                        ║
║ DEFAULT IMAGE SIZE: 800x600           ║
║ FORMATS: JPG, JPEG, JFIF              ║
║ PLATFORMS: Windows, macOS, Linux      ║
╚════════════════════════════════════════╝
```

---

**Ready to help students! 🎓🤖**
