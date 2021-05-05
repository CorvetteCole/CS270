# simpsh (Cole Gerdemann)
## About
This project was not a group project, although in my endeavor to assist other students I did share small portions of code from my project. These snippets were exclusively small utility methods such as the below:
```
string getCurrentDirectory() {
    char temp[PATH_MAX];
    return getcwd(temp, sizeof(temp));
}
```

## Usage
To compile this program, run `make` or `make all`. 
Execute it with `./simpsh`. To exit use Ctrl+D or type quit.

`make clean` is also implemented to make testing easier.


## Design
This project is separated in to three main files.

 - **simpsh.cpp**: This file contains the main() function and is the entrypoint for the shell. It contains a bare minimum of other things required to parse user input before handing it off elsewhere

 - **Variables.cpp/h**: This class is essentially a wrapper class for a map<string, string> that provides convenience methods and input validity checking for anything related to variables (listing, setting, unsetting, etc).

- **Commands.cpp/h**: In an effort to separate non-parsing related code out of simpsh.cpp, this class contains static methods that actually implement the commands specified in the design document, as well as a single public entrypoint `handleCommand`. This method checks the command token and then uses a dynamic function call to execute whatever command is requested. This helps keep the code modular and makes it much easier to add future commands. 

## Known Limitations
In a command such as this `PATH = $PATH:"/home/coleg/Documents 2/temp"`, The quotations will not be handled correctly. In general, quotations will be handled perfectly unless right up against a preceding character. This is a relatively minor limitation, and would probably not be so hard to fix if I had a few more minutes to work on this.

**WORKAROUND**: If you absolutely need to do something such as the above, you can set a temporary variable to the value, and then use that variable in place of the value. This will work as desired.

