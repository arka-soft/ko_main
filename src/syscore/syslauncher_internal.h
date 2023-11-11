#pragma once


#include <windows.h>
#include <iostream>
#include "syscore.h"


/**
 * @class NODE
 *
 * @brief A class representing a C-like module / object that can be dynamically loaded.
 * 
 * A Node is an object that represents a module/object that is:
 * - compiled separately into a shared library (DLL), with the special name: node_name.dll
 * - and exports 2 functions with the name node_name_setup and node_name_loop
 *  
 * Here's how to use it.
 * - Define NODE_SHARED_MEMORY in the launcher.h as the data you like to be shared across your nodes.
 * - Organize your code into two main functions called node_name_loop and node_name_setup.
 * - Compile the cpp file that includes these functions into a dll named node_name_master.dll
 * - Simply define the node by calling NODE("node_name").
 *   This will look for a node_name_master.dll in the working directory of a program.
 *   And it will try to dynamically load node_name_setup and node_name_loop functions.
 * - Enjoy dynamically loading your setup and loop functions using the provided utility functions.
 * 
 */
class NODE
{
    // DEFINITIONS
    typedef /*returns*/ void (*NODE_SETUP_FUNC)(/*takes*/ NODE_SHARED_MEMORY&);
    typedef /*returns*/ void (*NODE_LOOP_FUNC)(/*takes*/ NODE_SHARED_MEMORY&);

    // DATA
	public:
	NODE_SETUP_FUNC setup; // function ptr to the setup function. Called just like a method using node.setup 
	NODE_LOOP_FUNC  loop; // function ptr to the loop function. Called just like a method using node.loop

    private:
    HINSTANCE dll_handle; // handle to the node DLL

    std::string name; // name of the NODE, provided by the user.
    std::string master_dll_name; // name_master.dll by default
    std::string loaded_dll_name; // temp/name_slave.dll by default
    std::string loaded_dll_dir;  // temp  by default 

    std::string setup_func_name; // name of the setup function to load from the DLL. name_setup by default.
    std::string loop_func_name;  // name of the setup function to load from the DLL. name_loop by default.

    // METHODS
	public:
    /**
     * @brief Construct a new NODE using its name.
     *
     * @param name Name of the node. The class will load name_setup() and name_loop() from name.dll.
     */
	NODE(std::string name)
	{
        this->name = name;
        this->master_dll_name = name + "_master.dll";
        this->loaded_dll_dir =  "temp";
        this->loaded_dll_name = this->loaded_dll_dir + "/" + name + "_slave.dll";
        this->setup_func_name = name + "_setup";
        this->loop_func_name = name + "_loop";

        if(DEBUG)
        {
            std::cout << ">> Node Registered: "  << this->name << std::endl;
            std::cout << " - dll_name: "  << this->master_dll_name << "  : Make sure to build it to a dll.)" << std::endl;
            std::cout << " - setup_func_name: "  << this->setup_func_name << "  : Setup function to load from dll." << std::endl;
            std::cout << " - loop_func_name: "  << this->loop_func_name << "  : Loop function to load from dll." << std::endl;
        }

        load_node_dll();
	}
    /**
     * @brief Destruct a NODE.
     */
    ~NODE()
    {
        if (dll_handle) FreeLibrary(dll_handle);
    }


    /**
     * @brief Compares the last write timestamp of the node_name.dll to see whether it has changed.
     *
     * @return bool Returns true if the dll was changed since the last run.
     */
    bool is_master_dll_changed()
    {
        WIN32_FILE_ATTRIBUTE_DATA file_attribute_data;
        GetFileAttributesExA(master_dll_name.c_str(), GetFileExInfoStandard, &file_attribute_data);

        FILETIME static previous_last_write_time = file_attribute_data.ftLastWriteTime;
        FILETIME current_last_write_time = file_attribute_data.ftLastWriteTime;

        if (CompareFileTime(&current_last_write_time, &previous_last_write_time) != 0)
        {
            previous_last_write_time =  current_last_write_time;
            return true;
        }
        else
            return false;
    }

    /**
     * @brief Dynamically reloads the setup and loop functions from the DLL.
     *
     */
    void reload_dll()
    {
        if (dll_handle)
            FreeLibrary(dll_handle);
        load_node_dll();
    }

    private:
    /**
     * @brief Loads the setup and loop functions from the DLL.
     */
    void load_node_dll()
    {
        CreateDirectoryA(loaded_dll_dir.c_str(), NULL);
        CopyFileA(master_dll_name.c_str(), loaded_dll_name.c_str(), false);

        dll_handle = LoadLibraryA(loaded_dll_name.c_str());
        if (!dll_handle) {std::cout<< "Couldn't load the Dll." << std::endl;} // TODO: (SHA) handle error

        setup = (NODE_SETUP_FUNC) GetProcAddress(dll_handle, setup_func_name.c_str());
        if (!setup) 
        {
            std::cout<< "Couldn't load the setup function. Make sure it is: __declspec(dllexport) extern 'C'" << std::endl;
            setup = &setup_dummy;
        } // TODO: (SHA)  do something with this

        loop = (NODE_LOOP_FUNC) GetProcAddress(dll_handle, loop_func_name.c_str());
        if (!loop) 
        {
            std::cout<< "Couldn't load the loop function. Make sure it is: __declspec(dllexport) extern 'C'" << std::endl;
            loop = &loop_dummy;
        } // TODO: (SHA)  do something with this
    }

    /**
     * @brief Dummy functions to use if the actual setup function cannot be loaded
     */
    void static setup_dummy(NODE_SHARED_MEMORY& notused){/*does nothing*/} 
    void static loop_dummy(NODE_SHARED_MEMORY& notused){/*does nothing*/}

};
