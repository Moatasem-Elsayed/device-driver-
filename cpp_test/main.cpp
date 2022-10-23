#include <fstream>
#include <iostream>

class Driver
{
private:
    const std::string PATH = "/dev/test_file";
    std::fstream m_fd;

public:
    void WriteFile(std::string message);
    std::string ReadFile();
};
void Driver::WriteFile(std::string message)
{
    m_fd.open(PATH, std::ios::out);
    m_fd.write(message.c_str(), message.size());
    m_fd.close();
}
std::string Driver::ReadFile()
{
    std::string result;
    m_fd.open(PATH);
    std::getline(m_fd, result);
    m_fd.close();
    return result;
}

int main()
{
    std::string input;
    Driver d;
    std::cout << "the application is Running ..." << std::endl;
    std::cout << "please enter Data: ";
    std::getline(std::cin, input);
    d.WriteFile(input);
    std::cout << d.ReadFile() << std::endl;
    return 0;
}