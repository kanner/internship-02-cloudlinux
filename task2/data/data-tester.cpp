#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <algorithm>
 
bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

int get_reps(std::string param)
{
	try {
		int reps = std::stoi(param);
		return reps;
	}
	catch (std::invalid_argument)
	{
		std::cout << "Invalid reps" << std::endl;
	}
	catch (std::out_of_range)
	{
		std::cout << "Invalid reps" << std::endl;
	}
	exit(1);
}
 
int main(int argc, char **argv)
{
	int fd;
	int reps;
	if (argc != 3) {
		std::cout << "usage: ./a.out reps string" << std::endl;
		return 1;
	}
	if (!is_number(argv[1])) {
		std::cout << "usage: ./a.out reps string" << std::endl;
		return 1;
	}
	reps = get_reps(std::string(argv[1]));
	fd = open("/sys/kernel/debug/kernelcare/data", O_WRONLY);
	if (fd < 0) {
		perror("Data file could not be opened\n");
		return 1;
	}
	std::string buffer = "";
	for (int i = 0; i < reps; i++)
		buffer += std::string(argv[2]);
	int bytes_written = write(fd, buffer.c_str(), buffer.size());
	close(fd);
	std::cout << "Bytes written: " << bytes_written << std::endl;
	return (0);
}
