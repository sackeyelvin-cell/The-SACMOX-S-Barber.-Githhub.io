#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

// ============================================================================
// IMAGE DISPLAY FUNCTIONS (Cross-Platform)
// ============================================================================

// Function to open and display JPG images
void displayImage(string imagePath) {
    // Check if file exists
    if (!fs::exists(imagePath)) {
        cout << "\n⚠️  Image not found: " << imagePath << endl;
        cout << "Expected location: " << fs::absolute(imagePath) << endl;
        return;
    }
    
    #ifdef _WIN32
        // Windows: Use default image viewer
        string command = "start \"\" \"" + imagePath + "\"";
        cout << "\n🖼️  Opening image: " << imagePath << endl;
        system(command.c_str());
    #elif __APPLE__
        // macOS: Use Preview or default viewer
        string command = "open \"" + imagePath + "\"";
        cout << "\n🖼️  Opening image: " << imagePath << endl;
        system(command.c_str());
    #else
        // Linux: Use eog, feh, or other image viewers
        string command = "eog \"" + imagePath + "\" 2>/dev/null || feh \"" + imagePath + "\" 2>/dev/null || display \"" + imagePath + "\" 2>/dev/null";
        cout << "\n🖼️  Opening image: " << imagePath << endl;
        system(command.c_str());
    #endif
}

// Function to display multiple images in sequence
void displayImages(vector<string> imagePaths) {
    for (const auto& path : imagePaths) {
        displayImage(path);
        #ifdef _WIN32
            Sleep(2000);  // Wait 2 seconds between images
        #else
            sleep(2);
        #endif
    }
}

// Function to create/verify images directory
void createImagesDirectory() {
    string imgDir = "campus_images";
    
    if (!fs::exists(imgDir)) {
        try {
            fs::create_directory(imgDir);
            cout << "✅ Created 'campus_images' directory" << endl;
            cout << "📁 Place your JPG images here:" << endl;
            cout << "   - library.jpg" << endl;
            cout << "   - registration.jpg" << endl;
            cout << "   - advisor.jpg" << endl;
            cout << "   - wifi.jpg" << endl;
            cout << "   - campus_map.jpg" << endl;
            cout << "   - dining.jpg" << endl;
            cout << "   - student_id.jpg" << endl;
            cout << "   - financial_aid.jpg" << endl;
            cout << "   - clubs.jpg" << endl;
            cout << "   - tutoring.jpg" << endl;
        } catch (const exception& e) {
            cout << "⚠️  Could not create directory: " << e.what() << endl;
        }
    }
}

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================
vector<pair<string, string>> chatHistory;
int conversationCount = 0;

// Image paths structure
struct ImagePaths {
    string library = "campus_images/library.jpg";
    string registration = "campus_images/registration.jpg";
    string advisor = "campus_images/advisor.jpg";
    string wifi = "campus_images/wifi.jpg";
    string campus_map = "campus_images/campus_map.jpg";
    string dining = "campus_images/dining.jpg";
    string student_id = "campus_images/student_id.jpg";
    string financial_aid = "campus_images/financial_aid.jpg";
    string clubs = "campus_images/clubs.jpg";
    string tutoring = "campus_images/tutoring.jpg";
};

ImagePaths images;

// ============================================================================
// VOICE FUNCTIONS
// ============================================================================

void speak(string text) {
    string cleanText = text;
    size_t pos = 0;
    while ((pos = cleanText.find("@")) != string::npos) {
        cleanText.replace(pos, 1, "at");
    }
    
    #ifdef _WIN32
        string command = "PowerShell -Command \"Add-Type –AssemblyName System.Speech; "
                        "$speak = New-Object System.Speech.Synthesis.SpeechSynthesizer; "
                        "$speak.Rate = 1; "
                        "$speak.Speak('" + cleanText + "');\"";
        system(command.c_str());
    #elif __APPLE__
        string command = "say \"" + cleanText + "\" 2>/dev/null &";
        system(command.c_str());
    #else
        string command = "espeak \"" + cleanText + "\" 2>/dev/null || festival --tts <<< \"" + cleanText + "\" 2>/dev/null &";
        system(command.c_str());
    #endif
}

string getSpeechInput() {
    cout << "\n🎤 Listening... (speak now)" << endl;
    
    #ifdef _WIN32
        system("PowerShell -Command \"Add-Type -AssemblyName System.Speech; "
               "$recognizer = New-Object System.Speech.Recognition.SpeechRecognitionEngine; "
               "$recognizer.SetInputToDefaultAudioDevice(); "
               "$result = $recognizer.Recognize(5); "
               "if ($result -and $result.Text) { Write-Host $result.Text } else { Write-Host '' }\" > speech_input.txt 2>nul");
        
        FILE* file = fopen("speech_input.txt", "r");
        if (file) {
            char buffer[512];
            if (fgets(buffer, sizeof(buffer), file) != NULL) {
                fclose(file);
                string result(buffer);
                result.erase(result.find_last_not_of("\n\r") + 1);
                system("del speech_input.txt >nul 2>&1");
                if (!result.empty() && result != "No speech detected") {
                    return result;
                }
            }
            fclose(file);
        }
    #endif
    
    return "";
}

// ============================================================================
// DATE & TIME FUNCTIONS
// ============================================================================

string getCurrentDateTime() {
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%A, %B %d, %Y at %I:%M %p", timeinfo);
    return string(buffer);
}

string getCurrentTime() {
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%I:%M %p", timeinfo);
    return string(buffer);
}

string getDayOfWeek() {
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%A", timeinfo);
    return string(buffer);
}

// ============================================================================
// CHAT HISTORY FUNCTIONS
// ============================================================================

void addToHistory(string question, string answer) {
    chatHistory.push_back(make_pair(question, answer));
    conversationCount++;
}

void displayChatHistory() {
    if (chatHistory.empty()) {
        cout << "\n📋 Chat History is empty.\n" << endl;
        return;
    }
    
    cout << "\n" << "========================================" << endl;
    cout << "          📋 CHAT HISTORY (" << chatHistory.size() << " messages)" << endl;
    cout << "========================================\n" << endl;
    
    for (int i = 0; i < chatHistory.size(); i++) {
        cout << "[" << (i + 1) << "] You: " << chatHistory[i].first << endl;
        cout << "    Bot: " << chatHistory[i].second.substr(0, 80);
        if (chatHistory[i].second.length() > 80) cout << "...";
        cout << "\n" << endl;
    }
    
    cout << "========================================\n" << endl;
}

void saveChatHistory(string filename = "chat_history.txt") {
    ofstream file(filename);
    
    if (!file.is_open()) {
        cout << "⚠️  Could not save chat history." << endl;
        return;
    }
    
    file << "===== UNIBOT CHAT HISTORY WITH IMAGES =====" << endl;
    file << "Date: " << getCurrentDateTime() << endl;
    file << "Total Messages: " << chatHistory.size() << endl;
    file << "==========================================\n" << endl;
    
    for (int i = 0; i < chatHistory.size(); i++) {
        file << "[" << (i + 1) << "] USER: " << chatHistory[i].first << endl;
        file << "    BOT: " << chatHistory[i].second << "\n" << endl;
    }
    
    file.close();
    cout << "✅ Chat history saved to " << filename << endl;
}

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

string toLowerCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

string getRandomResponse(int type) {
    vector<string> greetings = {
        "Hello there!",
        "Hi! How can I help?",
        "Greetings! What do you need?",
        "Hey! Great to see you!"
    };
    
    vector<string> acknowledgments = {
        "Great question!",
        "That's an important question.",
        "Let me help you with that.",
        "Good thinking!"
    };
    
    vector<string> farewells = {
        "Good luck with your studies!",
        "Hope I helped! See you later!",
        "Best wishes for your journey here!",
        "Take care and enjoy campus life!"
    };
    
    if (type == 0) return greetings[rand() % greetings.size()];
    if (type == 1) return acknowledgments[rand() % acknowledgments.size()];
    if (type == 2) return farewells[rand() % farewells.size()];
    
    return "Have a great day!";
}

// ============================================================================
// GREETING & DISPLAY
// ============================================================================

void displayGreeting() {
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║   🎓 Welcome to UniBot v4.0 🎓       ║" << endl;
    cout << "║   With Custom JPG Campus Images      ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    
    cout << "\n✨ FEATURES:" << endl;
    cout << "   🎤 Voice Recognition" << endl;
    cout << "   🔊 Voice Output" << endl;
    cout << "   🖼️  Custom JPG Images" << endl;
    cout << "   📅 Date & Time" << endl;
    cout << "   📋 Chat History" << endl;
    cout << "   💾 Save Transcripts\n" << endl;
    
    string greeting = "Hello! I'm UniBot v4.0 with visual campus guides! Ask me about campus and I'll show you images!";
    cout << "📢 UniBot: " << greeting << endl;
    speak(greeting);
}

// ============================================================================
// MAIN CHATBOT LOGIC
// ============================================================================

pair<string, string> answerQuestion(string question) {
    string originalQuestion = question;
    question = toLowerCase(question);
    
    string acknowledgment = getRandomResponse(1);
    string answer = "";
    
    // LIBRARY
    if (question.find("library") != string::npos) {
        displayImage(images.library);
        answer = "The main library is located in the Academic Building, north side of campus. "
                "It's open Monday through Friday from 8 AM to 10 PM, Saturday 10 AM to 6 PM, "
                "and Sunday 12 PM to 8 PM. We also have 3 smaller study centers around campus!";
    }
    
    // REGISTRATION
    else if (question.find("register") != string::npos || 
             question.find("registration") != string::npos ||
             question.find("enroll") != string::npos) {
        displayImage(images.registration);
        answer = "You can register for classes through the Student Portal online. "
                "Login with your student ID and password. Course registration opens 2 weeks before "
                "each semester. Peak times are 8 to 10 AM, so try registering outside peak hours!";
    }
    
    // ADVISOR
    else if (question.find("advisor") != string::npos ||
             question.find("academic advisor") != string::npos) {
        displayImage(images.advisor);
        answer = "Your academic advisor is assigned based on your major. "
                "Visit the Student Success Center in Building C, Room 215. "
                "You can also call (555) 123-4567 or email advisors@university.edu. "
                "It's recommended to meet with your advisor each semester!";
    }
    
    // WiFi
    else if (question.find("wifi") != string::npos ||
             question.find("wi-fi") != string::npos ||
             question.find("password") != string::npos ||
             question.find("internet") != string::npos ||
             question.find("network") != string::npos) {
        displayImage(images.wifi);
        answer = "WiFi is free for all students! The network name is UniCampus. "
                "Go to Settings, WiFi, Select UniCampus, then enter your student ID and password. "
                "If you have issues, visit IT Support in the Tech Building or call (555) 555-0123.";
    }
    
    // CAMPUS MAP
    else if (question.find("where") != string::npos && 
             (question.find("campus") != string::npos || 
              question.find("building") != string::npos ||
              question.find("location") != string::npos)) {
        displayImage(images.campus_map);
        answer = "Our campus is located at 123 University Avenue. "
                "The main entrance is near the Student Center. "
                "Campus tours are available Tuesdays and Thursdays at 2 PM!";
    }
    
    // DINING
    else if (question.find("food") != string::npos ||
             question.find("dining") != string::npos ||
             question.find("cafeteria") != string::npos ||
             question.find("restaurant") != string::npos ||
             question.find("eat") != string::npos) {
        displayImage(images.dining);
        answer = "We have multiple dining options: Main Cafeteria in the Student Center, "
                "Tech Cafe for quick bites, and the Dining Hall. Most accept student meal plans. "
                "Meal plans are available online. Check current hours!";
    }
    
    // LIBRARY HOURS
    else if (question.find("hour") != string::npos || 
             question.find("open") != string::npos ||
             question.find("close") != string::npos) {
        displayImage(images.library);
        answer = "Main Library Hours: Monday through Friday 8 AM to 10 PM, Saturday 10 AM to 6 PM, "
                "Sunday 12 PM to 8 PM. Study centers have extended hours until midnight most days!";
    }
    
    // STUDENT ID
    else if (question.find("student id") != string::npos ||
             question.find("student card") != string::npos ||
             question.find("id card") != string::npos ||
             question.find("identification") != string::npos) {
        displayImage(images.student_id);
        answer = "Your student ID card is essential! Pick it up at the Student ID Center "
                "in the Student Center, Room 101. You'll need it for library access, meal plans, "
                "and campus events. Processing takes about 15 minutes!";
    }
    
    // FINANCIAL AID
    else if (question.find("financial aid") != string::npos ||
             question.find("scholarships") != string::npos ||
             question.find("tuition") != string::npos ||
             question.find("aid") != string::npos ||
             question.find("grants") != string::npos) {
        displayImage(images.financial_aid);
        answer = "Visit the Financial Aid Office in Building F, Room 310 for help with: "
                "FAFSA completion, scholarships, loans, and payment plans. "
                "Office hours are Monday through Friday, 9 AM to 4 PM!";
    }
    
    // CLUBS
    else if (question.find("clubs") != string::npos ||
             question.find("organization") != string::npos ||
             question.find("activities") != string::npos ||
             question.find("student life") != string::npos ||
             question.find("join") != string::npos) {
        displayImage(images.clubs);
        answer = "We have 150+ student clubs and organizations! Browse clubs online "
                "or visit the Student Life Office in the Student Center. Club fair happens during orientation!";
    }
    
    // TUTORING
    else if (question.find("tutor") != string::npos ||
             (question.find("help") != string::npos && question.find("study") != string::npos) ||
             question.find("academic support") != string::npos) {
        displayImage(images.tutoring);
        answer = "Free tutoring is available at the Tutoring Center in Building A, Room 105. "
                "We offer help in Math, Writing, Science, and more. "
                "Hours: Monday through Thursday 9 AM to 7 PM, Friday 9 AM to 3 PM!";
    }
    
    // DEFAULT
    else {
        answer = "I'm not sure about that one, but I can help! "
                "For questions I can't answer, visit the Student Services Office in Building B, Room 200.";
    }
    
    return make_pair(acknowledgment + " " + answer, answer);
}

// ============================================================================
// MAIN PROGRAM
// ============================================================================

int main() {
    srand(time(0));
    
    string userInput;
    bool useVoice = false;
    
    // Create images directory
    createImagesDirectory();
    
    displayGreeting();
    
    cout << "\n📚 CAMPUS TOPICS I CAN HELP WITH:" << endl;
    cout << "   1. Where is the library?" << endl;
    cout << "   2. How do I register for classes?" << endl;
    cout << "   3. Who is my academic advisor?" << endl;
    cout << "   4. What is the WiFi password?" << endl;
    cout << "   5. Where are the campus buildings?" << endl;
    cout << "   6. What dining options are available?" << endl;
    cout << "   7. What are the library hours?" << endl;
    cout << "   8. How do I get a student ID card?" << endl;
    cout << "   9. How do I apply for financial aid?" << endl;
    cout << "   10. What clubs and activities are available?" << endl;
    cout << "   11. Where can I get tutoring help?" << endl;
    
    cout << "\n⚙️  SPECIAL COMMANDS:" << endl;
    cout << "   'voice' - Toggle voice on/off" << endl;
    cout << "   'history' - View chat history" << endl;
    cout << "   'save' - Save chat history to file" << endl;
    cout << "   'quit' or 'exit' - Leave\n" << endl;
    
    // Main conversation loop
    while (true) {
        cout << "\n───────────────────────────────────────" << endl;
        
        if (useVoice) {
            cout << "🎤 You (speak or type): ";
            userInput = getSpeechInput();
            
            if (userInput.empty()) {
                cout << "📝 Type your question: ";
                getline(cin, userInput);
            } else {
                cout << "You said: " << userInput << endl;
            }
        } else {
            cout << "📝 You: ";
            getline(cin, userInput);
        }
        
        // Remove extra whitespace
        userInput.erase(0, userInput.find_first_not_of(" \t"));
        
        // Check for voice toggle
        if (toLowerCase(userInput) == "voice") {
            useVoice = !useVoice;
            string message = useVoice ? 
                "Voice mode enabled." : 
                "Voice mode disabled.";
            cout << "🤖 UniBot: " << message << endl;
            if (useVoice) speak(message);
            continue;
        }
        
        // Check for history command
        if (toLowerCase(userInput) == "history") {
            displayChatHistory();
            continue;
        }
        
        // Check for save command
        if (toLowerCase(userInput) == "save") {
            saveChatHistory();
            continue;
        }
        
        // Check for exit command
        if (toLowerCase(userInput) == "quit" || 
            toLowerCase(userInput) == "exit" || 
            toLowerCase(userInput) == "bye") {
            string farewell = getRandomResponse(2);
            cout << "\n🤖 UniBot: " << farewell << " 👋" << endl;
            speak(farewell);
            cout << "\n📊 Session Summary:" << endl;
            cout << "   Total Questions: " << chatHistory.size() << endl;
            cout << "   Session Ended: " << getCurrentDateTime() << endl;
            cout << "╔════════════════════════════════════════╗" << endl;
            cout << "║  Thank you for using UniBot! Goodbye!  ║" << endl;
            cout << "╚════════════════════════════════════════╝\n" << endl;
            break;
        }
        
        // Skip empty input
        if (userInput.empty()) {
            cout << "🤖 UniBot: Please ask me a question!\n" << endl;
            if (useVoice) speak("Please ask me a question!");
            continue;
        }
        
        // Get and display the response
        pair<string, string> response = answerQuestion(userInput);
        cout << "\n🤖 UniBot: " << response.first << endl;
        
        // Add to chat history
        addToHistory(userInput, response.second);
        
        // Speak the response if voice is enabled
        if (useVoice) {
            speak(response.first);
        }
    }
    
    return 0;
}
