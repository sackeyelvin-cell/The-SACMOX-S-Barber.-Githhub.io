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

using namespace std;

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================
vector<pair<string, string>> chatHistory;  // Store conversation history
int conversationCount = 0;                  // Track number of interactions

// ============================================================================
// CROSS-PLATFORM VOICE FUNCTIONS
// ============================================================================

// Function to speak text using platform-specific APIs
void speak(string text) {
    // Replace problematic characters for voice output
    string cleanText = text;
    size_t pos = 0;
    while ((pos = cleanText.find("@")) != string::npos) {
        cleanText.replace(pos, 1, "at");
    }
    
    #ifdef _WIN32
        // Windows: Use SAPI (Speech API)
        string command = "PowerShell -Command \"Add-Type –AssemblyName System.Speech; "
                        "$speak = New-Object System.Speech.Synthesis.SpeechSynthesizer; "
                        "$speak.Rate = 1; "
                        "$speak.Speak('" + cleanText + "');\"";
        system(command.c_str());
    #elif __APPLE__
        // macOS: Use 'say' command
        string command = "say \"" + cleanText + "\" 2>/dev/null &";
        system(command.c_str());
    #else
        // Linux: Use 'espeak' or 'festival'
        string command = "espeak \"" + cleanText + "\" 2>/dev/null || festival --tts <<< \"" + cleanText + "\" 2>/dev/null &";
        system(command.c_str());
    #endif
}

// Function to get speech input using platform-specific APIs
string getSpeechInput() {
    cout << "\n🎤 Listening... (speak now)" << endl;
    
    #ifdef _WIN32
        // Windows: Use PowerShell with Windows.Media.Speech
        system("PowerShell -Command \"Add-Type -AssemblyName System.Speech; "
               "$recognizer = New-Object System.Speech.Recognition.SpeechRecognitionEngine; "
               "$recognizer.SetInputToDefaultAudioDevice(); "
               "$result = $recognizer.Recognize(5); "
               "if ($result -and $result.Text) { Write-Host $result.Text } else { Write-Host '' }\" > speech_input.txt 2>nul");
        
        // Read the output
        FILE* file = fopen("speech_input.txt", "r");
        if (file) {
            char buffer[512];
            if (fgets(buffer, sizeof(buffer), file) != NULL) {
                fclose(file);
                string result(buffer);
                // Remove newline
                result.erase(result.find_last_not_of("\n\r") + 1);
                system("del speech_input.txt >nul 2>&1");
                if (!result.empty() && result != "No speech detected") {
                    return result;
                }
            }
            fclose(file);
            system("del speech_input.txt >nul 2>&1");
        }
    #elif __APPLE__
        // macOS: Use native speech recognition
        system("(osascript -e 'activate application \"Finder\"' && "
               "osascript -e 'tell application \"System Events\" to "
               "keystroke \"\\\" using {command down, option down, shift down}\"' ) 2>/dev/null");
        cout << "📢 Please enable Voice Control in System Preferences > Accessibility" << endl;
        return "";
    #else
        // Linux: Use 'pocketsphinx' if available
        system("pocketsphinx_continuous -inmic yes -time yes 2>/dev/null | head -n 1 > speech_input.txt &");
        cout << "📢 Ensure 'pocketsphinx' is installed: sudo apt-get install pocketsphinx" << endl;
        return "";
    #endif
    
    return "";
}

// ============================================================================
// DATE & TIME FUNCTIONS
// ============================================================================

// Function to get current date and time
string getCurrentDateTime() {
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%A, %B %d, %Y at %I:%M %p", timeinfo);
    return string(buffer);
}

// Function to get current time only
string getCurrentTime() {
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%I:%M %p", timeinfo);
    return string(buffer);
}

// Function to get day of week
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

// Function to add to chat history
void addToHistory(string question, string answer) {
    chatHistory.push_back(make_pair(question, answer));
    conversationCount++;
}

// Function to display chat history
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

// Function to save chat history to file
void saveChatHistory(string filename = "chat_history.txt") {
    ofstream file(filename);
    
    if (!file.is_open()) {
        cout << "⚠️  Could not save chat history." << endl;
        return;
    }
    
    file << "===== UNIBOT CHAT HISTORY =====" << endl;
    file << "Date: " << getCurrentDateTime() << endl;
    file << "Total Messages: " << chatHistory.size() << endl;
    file << "================================\n" << endl;
    
    for (int i = 0; i < chatHistory.size(); i++) {
        file << "[" << (i + 1) << "] USER: " << chatHistory[i].first << endl;
        file << "    BOT: " << chatHistory[i].second << "\n" << endl;
    }
    
    file.close();
    cout << "✅ Chat history saved to " << filename << endl;
}

// ============================================================================
// CHATBOT CORE FUNCTIONS
// ============================================================================

// Function to convert string to lowercase for case-insensitive comparison
string toLowerCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// Random response generator
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

// Function to display the chatbot greeting with voice
void displayGreeting() {
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║   🎓 Welcome to UniBot v2.0 🎓       ║" << endl;
    cout << "║   Your AI-Powered Student Assistant   ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    
    cout << "\n✨ FEATURES:" << endl;
    cout << "   🎤 Voice Recognition - Speak your questions" << endl;
    cout << "   🔊 Voice Output - Listen to answers" << endl;
    cout << "   📅 Date & Time - Know what day it is" << endl;
    cout << "   📋 Chat History - Review conversation" << endl;
    cout << "   🎲 Random Responses - Dynamic interactions" << endl;
    cout << "   📝 Text Input - Type if voice unavailable\n" << endl;
    
    string timeGreeting = "Good " + (stoi(getCurrentTime().substr(0, 2)) < 12 ? 
                         "morning" : (stoi(getCurrentTime().substr(0, 2)) < 18 ? 
                         "afternoon" : "evening")) + "!";
    
    string greeting = "Hello! I'm UniBot, your virtual student assistant. "
                     "I can help you with campus information. Today is " + getDayOfWeek() + 
                     " and the current time is " + getCurrentTime() + ". " + getRandomResponse(0);
    
    cout << "📢 UniBot: " << greeting << endl;
    speak(greeting);
}

// Function to answer questions based on user input
pair<string, string> answerQuestion(string question) {
    string originalQuestion = question;
    question = toLowerCase(question);
    
    string acknowledgment = getRandomResponse(1);
    string answer = "";
    
    // Check for date and time questions
    if (question.find("what") != string::npos && question.find("time") != string::npos) {
        answer = "The current time is " + getCurrentTime() + ".";
    }
    else if (question.find("what") != string::npos && question.find("date") != string::npos) {
        answer = "Today is " + getCurrentDateTime() + ".";
    }
    else if (question.find("what day") != string::npos) {
        answer = "Today is " + getDayOfWeek() + ".";
    }
    else if (question.find("library") != string::npos) {
        vector<string> responses = {
            "The main library is located in the Academic Building, north side of campus. "
            "It's open Monday through Friday from 8 AM to 10 PM, Saturday 10 AM to 6 PM, "
            "and Sunday 12 PM to 8 PM. We also have 3 smaller study centers around campus!",
            
            "Looking for the library? Head to the Academic Building on the north side. "
            "Weekday hours are 8 AM to 10 PM. Perfect for studying and researching!"
        };
        answer = responses[rand() % responses.size()];
    }
    
    else if (question.find("register") != string::npos || 
             question.find("registration") != string::npos ||
             question.find("enroll") != string::npos) {
        vector<string> responses = {
            "You can register for classes through the Student Portal online. "
            "Login with your student ID and password. Course registration opens 2 weeks before "
            "each semester. Peak times are 8 to 10 AM, so try registering outside peak hours!",
            
            "Registration is handled through our online Student Portal. "
            "You'll need your student ID. Mark your calendar for registration day!"
        };
        answer = responses[rand() % responses.size()];
    }
    
    else if (question.find("advisor") != string::npos ||
             question.find("academic advisor") != string::npos) {
        vector<string> responses = {
            "Your academic advisor is assigned based on your major. "
            "Visit the Student Success Center in Building C, Room 215. "
            "It's recommended to meet with your advisor each semester!",
            
            "Need to find your advisor? Head to the Student Success Center. "
            "Advisors are there to help you plan your academic journey!"
        };
        answer = responses[rand() % responses.size()];
    }
    
    else if (question.find("wifi") != string::npos ||
             question.find("wi-fi") != string::npos ||
             question.find("password") != string::npos ||
             question.find("internet") != string::npos ||
             question.find("network") != string::npos) {
        vector<string> responses = {
            "WiFi is free for all students! The network name is UniCampus. "
            "Go to Settings, WiFi, Select UniCampus, then enter your student ID and password. "
            "If you have issues, visit IT Support in the Tech Building!",
            
            "Connect to UniCampus WiFi using your student credentials. "
            "It's available all over campus for free!"
        };
        answer = responses[rand() % responses.size()];
    }
    
    else if (question.find("where") != string::npos && 
             (question.find("campus") != string::npos || 
              question.find("building") != string::npos ||
              question.find("location") != string::npos)) {
        vector<string> responses = {
            "Our campus is located at 123 University Avenue. "
            "The main entrance is near the Student Center. "
            "Campus tours are available Tuesdays and Thursdays at 2 PM!",
            
            "Welcome to campus! We're centrally located with easy access. "
            "Check out our campus map online for directions!"
        };
        answer = responses[rand() % responses.size()];
    }
    
    else if (question.find("food") != string::npos ||
             question.find("dining") != string::npos ||
             question.find("cafeteria") != string::npos ||
             question.find("restaurant") != string::npos ||
             question.find("eat") != string::npos) {
        vector<string> responses = {
            "We have multiple dining options: Main Cafeteria in the Student Center, "
            "Tech Cafe for quick bites, and the Dining Hall. Most accept student meal plans. "
            "Meal plans are available online!",
            
            "Hungry? Check out our cafeteria, tech cafe, or dining hall. "
            "All accept meal plans!"
        };
        answer = responses[rand() % responses.size()];
    }
    
    else if (question.find("hour") != string::npos || 
             question.find("open") != string::npos ||
             question.find("close") != string::npos) {
        vector<string> responses = {
            "Main Library Hours: Monday through Friday 8 AM to 10 PM, Saturday 10 AM to 6 PM, "
            "Sunday 12 PM to 8 PM. Study centers have extended hours until midnight most days!",
            
            "The library opens early and stays open late for students. "
            "Check online for specific hours!"
        };
        answer = responses[rand() % responses.size()];
    }
    
    else if (question.find("student id") != string::npos ||
             question.find("student card") != string::npos ||
             question.find("id card") != string::npos ||
             question.find("identification") != string::npos) {
        vector<string> responses = {
            "Your student ID card is essential! Pick it up at the Student ID Center "
            "in the Student Center, Room 101. You'll need it for library access, meal plans, "
            "and campus events. Processing takes about 15 minutes!",
            
            "Get your student ID at the ID Center. It's your key to campus life!"
        };
        answer = responses[rand() % responses.size()];
    }
    
    else if (question.find("financial aid") != string::npos ||
             question.find("scholarships") != string::npos ||
             question.find("tuition") != string::npos ||
             question.find("aid") != string::npos ||
             question.find("grants") != string::npos) {
        vector<string> responses = {
            "Visit the Financial Aid Office in Building F, Room 310 for help with: "
            "FAFSA, scholarships, loans, and payment plans. Office hours are Monday through Friday, 9 AM to 4 PM!",
            
            "Financial aid questions? The Financial Aid Office is ready to help. "
            "Don't worry, we've got options for everyone!"
        };
        answer = responses[rand() % responses.size()];
    }
    
    else if (question.find("clubs") != string::npos ||
             question.find("organization") != string::npos ||
             question.find("activities") != string::npos ||
             question.find("student life") != string::npos ||
             question.find("join") != string::npos) {
        vector<string> responses = {
            "We have 150 plus student clubs and organizations! Browse clubs online "
            "or visit the Student Life Office in the Student Center. Club fair happens during orientation!",
            
            "Want to get involved? We have tons of clubs and activities for every interest. "
            "Check them out!"
        };
        answer = responses[rand() % responses.size()];
    }
    
    else if (question.find("tutor") != string::npos ||
             (question.find("help") != string::npos && question.find("study") != string::npos) ||
             question.find("academic support") != string::npos) {
        vector<string> responses = {
            "Free tutoring is available at the Tutoring Center in Building A, Room 105. "
            "We offer help in Math, Writing, Science, and more. Hours: Monday through Thursday 9 AM to 7 PM!",
            
            "Struggling with a subject? Our tutoring center is here to help with one on one sessions!"
        };
        answer = responses[rand() % responses.size()];
    }
    
    // Default response for unrecognized questions
    else {
        vector<string> responses = {
            "I'm not sure about that one, but I can help! "
            "For questions I can't answer, visit the Student Services Office in Building B, Room 200, "
            "or call the main campus line. Our staff is always happy to help!",
            
            "That's a great question! I don't have that information, but the Student Services team can help. "
            "Stop by anytime!",
            
            "I'm still learning! Please contact the Student Services Office for more specialized questions."
        };
        answer = responses[rand() % responses.size()];
    }
    
    return make_pair(acknowledgment + " " + answer, answer);
}

// ============================================================================
// MAIN PROGRAM
// ============================================================================

int main() {
    srand(time(0));  // Seed random number generator
    
    string userInput;
    bool useVoice = true;
    
    displayGreeting();
    
    // Display menu of example questions
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
    cout << "   'time' or 'what time is it?' - Get current time" << endl;
    cout << "   'date' or 'what date is it?' - Get current date" << endl;
    cout << "   'history' - View chat history" << endl;
    cout << "   'save' - Save chat history to file" << endl;
    cout << "   'voice' - Toggle voice on/off" << endl;
    cout << "   'quit' or 'exit' - Leave\n" << endl;
    
    // Main conversation loop
    while (true) {
        cout << "\n───────────────────────────────────────" << endl;
        
        if (useVoice) {
            cout << "🎤 You (speak or type): ";
            userInput = getSpeechInput();
            
            // Fallback to text if voice not available or empty
            if (userInput.empty() || userInput == "unsupported" || userInput == "manual_input") {
                cout << "📝 Type your question: ";
                getline(cin, userInput);
            } else {
                cout << "You said: " << userInput << endl;
            }
        } else {
            cout << "📝 You (type): ";
            getline(cin, userInput);
        }
        
        // Remove extra whitespace
        userInput.erase(0, userInput.find_first_not_of(" \t"));
        
        // Check for voice toggle
        if (toLowerCase(userInput) == "voice") {
            useVoice = !useVoice;
            string message = useVoice ? 
                "Voice mode enabled. Speak your questions!" : 
                "Voice mode disabled. Type your questions.";
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
            cout << "   Total Questions Asked: " << chatHistory.size() << endl;
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
