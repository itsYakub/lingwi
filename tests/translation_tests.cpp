#include <gtest/gtest.h>
#include <cstdlib>
#include <sstream>
#include <string>

std::string executeCommand(const std::string& command, bool captureStderr = false)
{
    std::string result;
    char buffer[128];
    std::string fullCommand = "../../" + command;
    if (captureStderr) {
        fullCommand += " 2>&1";
    }
    FILE* pipe = popen(fullCommand.c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("Failed to run command.");
    }
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);
    return result;
}

TEST(TranslationTest1, ValidTranslationFromDefault)
{
    // english to spanish
    std::string command = "./lingwi -t es \"hello girl\"";
    std::string output = executeCommand(command);
    EXPECT_TRUE(strcasecmp(output.c_str(), "hola chica\n") == 0);

    // english to russian
    command = "./lingwi -t ru \"the program is being tested here\"";
    output = executeCommand(command);
    EXPECT_TRUE(strcasecmp(output.c_str(), "программа тестируется здесь\n") == 0 || 
        strcasecmp(output.c_str(), "Программа проверяется здесь\n") == 0);

    // english to polish
    command = "./lingwi -t pl \"hello from the lingwi app\"";
    output = executeCommand(command);
    EXPECT_TRUE(strcasecmp(output.c_str(), "witam z aplikacji lingwi\n") == 0 || 
                strcasecmp(output.c_str(), "witaj z aplikacji lingwi\n") == 0);

    // english to chinese
    command = "./lingwi -t zh \"chinese is crazy\"";
    output = executeCommand(command);
    EXPECT_TRUE(strcasecmp(output.c_str(), "中国人疯了\n") == 0 || strcasecmp(output.c_str(), "中国人很疯狂\n") == 0);
}

TEST(TranslationTest2, ValidTranslationSourceToTarget)
{
    // italian to english
    std::string command = "./lingwi -s it -t en \"Ciao, la carbonara non si fa con la panna.\"";
    std::string output = executeCommand(command);
    EXPECT_TRUE(strcasecmp(output.c_str(), "Hi, carbonara is not made with cream.\n") == 0 || 
                strcasecmp(output.c_str(), "Hi, carbonara is not done with cream.\n") == 0);

    // german to english
    command = "./lingwi -s en -t de \"I need forty-two apples.\"";
    output = executeCommand(command);
    EXPECT_TRUE(strcasecmp(output.c_str(), "Ich brauche zweiundvierzig Äpfel.\n") == 0);

    // spanish to english
    command = "./lingwi -s es -t en \"¿Dónde está la biblioteca?\"";
    output = executeCommand(command);
    EXPECT_TRUE(strcasecmp(output.c_str(), "Where is the library?\n") == 0);
}


// TEST(TranslationTest3, InvalidSourceLanguage)
// {
//     std::string command = "./lingwi -s xx -t en \"yessirski\"";
//     std::string output = executeCommand(command, true);
//     EXPECT_TRUE(strcasecmp(output.c_str(), "ERR.: Invalid langcode: xx\n") == 0);

//     command = "./lingwi -s en -t fd \"\"";
//     output = executeCommand(command, true);
//     EXPECT_TRUE(strcasecmp(output.c_str(), "ERR.: Invalid langcode: fd\n") == 0);

//     command = "./lingwi -t jajko \"jajka\"";
//     output = executeCommand(command, true);
//     EXPECT_TRUE(strcasecmp(output.c_str(), "ERR.: Invalid langcode: jajka") == 0);
// }

TEST(TranslationTest4, PunctuationHandling)
{
    // russian /w punctuation
    std::string command = "./lingwi -t ru \"She said: 'I'll be there soon.'\"";
    std::string output = executeCommand(command);
    EXPECT_TRUE(strcasecmp(output.c_str(), "Она сказала: Я скоро буду там.\n") == 0);

    // multiple punctuation (comma + question mark)
    command = "./lingwi -t es \"Hello, world! How are you today?\"";
    output = executeCommand(command);
    EXPECT_TRUE(strcasecmp(output.c_str(), "¡Hola Mundo! ¿Cómo estás hoy?\n") == 0);

    // spanish exclamation marks + special characters
    command = "./lingwi -t es \"The cost is $20 & it's worth it!\"";
    output = executeCommand(command);
    EXPECT_TRUE(strcasecmp(output.c_str(), "¡El costo es y vale la pena!\n") == 0);
    
    // spanish question marks
    command = "./lingwi -t es \"I don't know... what happened?\"";
    output = executeCommand(command);
    EXPECT_TRUE(strcasecmp(output.c_str(), "No sé... ¿qué pasó?\n") == 0 || strcasecmp(output.c_str(), "No sé... lo ¿qué pasó?\n") == 0
     || strcasecmp(output.c_str(), "No sé ... ¿qué pasó?\n") == 0 || strcasecmp(output.c_str(), "No sé ... lo ¿qué pasó?\n") == 0);
}