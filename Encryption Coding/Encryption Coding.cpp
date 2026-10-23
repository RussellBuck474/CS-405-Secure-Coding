// Encryption.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <ctime>

/// <summary>
/// encrypt or decrypt a source string using the provided key
/// </summary>
/// <param name="source">input string to process</param>
/// <param name="key">key to use in encryption / decryption</param>
/// <returns>transformed string</returns>
std::string encrypt_decrypt(const std::string& source, const std::string& key)
{
    // get lengths now instead of calling the function every time.
    // this would have most likely been inlined by the compiler, but design for performance.
    const auto key_length = key.length();
    const auto source_length = source.length();

    // assert that our input data is good
    assert(key_length > 0);
    assert(source_length > 0);

    std::string output = source;

    // loop through the source string char by char
    for (size_t i = 0; i < source_length; ++i)
    {
        // XOR each source character with the corresponding key character.
        // Modulo wraps the key index when the source is longer than the key.
        output[i] = source[i] ^ key[i % key_length];
    }

    // our output length must equal our source length
    assert(output.length() == source_length);

    // return the transformed string
    return output;
}

std::string read_file(const std::string& filename)
{
    // open the file for reading
    std::ifstream input_file(filename);

    // verify file opened successfully
    if (!input_file)
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return "";
    }

    // read entire file contents into a stringstream
    std::stringstream buffer;
    buffer << input_file.rdbuf();

    // return file contents as a string
    return buffer.str();
}

std::string get_student_name(const std::string& string_data)
{
    std::string student_name;

    // find the first newline
    size_t pos = string_data.find('\n');

    // did we find a newline
    if (pos != std::string::npos)
    {
        // we did, so copy that substring as the student name
        student_name = string_data.substr(0, pos);
    }

    return student_name;
}

void save_data_file(
    const std::string& filename,
    const std::string& student_name,
    const std::string& key,
    const std::string& data)
{
    // create output file
    std::ofstream output_file(filename);

    // verify file was created successfully
    if (!output_file)
    {
        std::cerr << "Error creating file: " << filename << std::endl;
        return;
    }

    // get current date/time
    time_t now = time(nullptr);

    tm local_time;

#ifdef _WIN32
    localtime_s(&local_time, &now);
#else
    local_time = *localtime(&now);
#endif

    // file format
    // Line 1: student name
    // Line 2: timestamp (yyyy-mm-dd)
    // Line 3: key used
    // Line 4+: data

    output_file << student_name << std::endl;
    output_file << std::put_time(&local_time, "%Y-%m-%d") << std::endl;
    output_file << key << std::endl;
    output_file << data;

    output_file.close();
}

int main()
{
    std::cout << "Encryption Decryption Test!" << std::endl;

    // input file format
    // Line 1: <students name>
    // Line 2: <Lorem Ipsum Generator website used>
    // Lines 3+: <lorem ipsum generated with 3 paragraphs>

    const std::string file_name = "inputdatafile.txt";
    const std::string encrypted_file_name = "encrypteddatafile.txt";
    const std::string decrypted_file_name = "decrytpteddatafile.txt";

    const std::string source_string = read_file(file_name);

    const std::string key = "password";

    // get the student name from the data file
    const std::string student_name = get_student_name(source_string);

    // encrypt source_string with key
    const std::string encrypted_string =
        encrypt_decrypt(source_string, key);

    // save encrypted_string to file
    save_data_file(
        encrypted_file_name,
        student_name,
        key,
        encrypted_string);

    // decrypt encrypted_string with key
    const std::string decrypted_string =
        encrypt_decrypt(encrypted_string, key);

    // save decrypted_string to file
    save_data_file(
        decrypted_file_name,
        student_name,
        key,
        decrypted_string);

    std::cout
        << "Read File: " << file_name
        << " - Encrypted To: " << encrypted_file_name
        << " - Decrypted To: " << decrypted_file_name
        << std::endl;

    // students submit input file, encrypted file,
    // decrypted file, source code file, and key used
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu