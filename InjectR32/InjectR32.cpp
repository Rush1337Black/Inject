#pragma once

#include "Process.cpp"
#include "Internal.cpp"

void Cleanup(const std::string message) {
   std::cout << message << std::endl;
   system("pause");
   ExitProcess(0);
}

std::string RandomString(const size_t length){
   std::string r;
   static const char bet[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYabcdefghijklmnopqrstuvwxyzZ1234567890" };
   srand((unsigned)time(NULL) * 5);
   for (int i = 0; i < length; ++i)
      r += bet[rand() % (sizeof(bet) - 1)];
   return r;
}

int main()
{
    SetConsoleTitleA(RandomString(26).c_str());

    Process process = Process();
    Internal internal = Internal();
   
    DWORD ProcessId = process.GetId("csgo.exe");

    while(!ProcessId){
       Cleanup("No ProcessId Found.");
    }

    HANDLE Game = process.Open();
    uintptr_t ModuleBase = process.GetModuleBase("client.dll");

    std::cout << "csgo.exe -> " << std::dec << ProcessId << std::endl;
    std::cout << "NTOpenFile -> " << std::dec << internal.GetNTOpenFile() << std::endl;
    std::cout << "process.Open -> " << std::dec << Game << std::endl;
    std::cout << "ModuleBase -> " << std::dec << ModuleBase << std::endl;

    if (internal.CheckFileExist("dhw.dll")){

       if (!internal.ExecuteBypass(Game)){
          Cleanup("Cannot Bypass...");
       }

       if (process.LoadLibraryInject("dhw.dll")){

          internal.Backup(Game);
          std::cout << "Injected" << std::endl;
          system("pause");
          ExitProcess(0);
       }
       else{
          internal.Backup(Game);
          Cleanup("Injection Failed.");
       }
    }
    return 0;
}
