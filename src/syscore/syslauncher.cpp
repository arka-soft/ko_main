#include "syscore.h"
#include "syslauncher_internal.h"

int main() {
    NODE_SHARED_MEMORY data;
    NODE config_node{"config"};

    config_node.setup(data);

    while(1)
    {   
        if(config_node.is_master_dll_changed())
        {
            config_node.reload_dll();
            config_node.setup(data);
        }
        config_node.loop(data);
        Sleep(50);
    }
}





