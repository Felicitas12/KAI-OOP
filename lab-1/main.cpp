#include <windows.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <cstdlib>
#include <string>
#include <iostream>
#pragma comment(lib, "mono-2.0.lib")

int main(int argc, char* argv[])
{
#pragma region Load and compile the script
    std::string scriptPath(R"(Lines.cs)");
    std::string command = "\"" MONO_HOME "/bin/mcs\" " + scriptPath + " -target:library";

    //Compile the script
    system(command.c_str());
#pragma endregion

//#pragma region Init mono runtime
    mono_set_dirs(MONO_HOME "/lib",
        MONO_HOME "/etc");

    //Init a domain
    MonoDomain *domain;
    domain = mono_jit_init("MonoScriptTry");
    if (!domain)
    {
        std::cout << "mono_jit_init failed" << std::endl;
        return 1;
    }


    //Open a assembly in the domain
    MonoAssembly *assembly = mono_domain_assembly_open(domain, "Lines.dll");
    if (!assembly)
    {
        std::cout << "mono_domain_assembly_open failed" << std::endl;
        return 1;
    }

    //Get a image from the assembly
    MonoImage* image = mono_assembly_get_image(assembly);
    if (!image)
    {
        std::cout << "mono_assembly_get_image failed" << std::endl;
        return 1;
    }
#pragma endregion

    MonoClass* klass = mono_class_from_name(image, "", "Lines");
    MonoMethodDesc* desc = mono_method_desc_new("Lines:CreateInstance(string)", NULL);
    MonoMethod* createMethod = mono_method_desc_search_in_image(desc, image);

    if (!createMethod)
    {
        std::cerr << "Метод CreateInstance не знайдено!" << std::endl;
        return 1;
    }

    MonoString* inputString = mono_string_new(domain, "hello");
    void* args[1] = { inputString };
    MonoObject* linesInstance = mono_runtime_invoke(createMethod, nullptr, args, nullptr);

    // Виклик GetValue()
    MonoMethodDesc* getValueDesc = mono_method_desc_new("Lines:GetValue()", NULL);
    MonoMethod* getValueMethod = mono_method_desc_search_in_image(getValueDesc, image);
    MonoObject* getValueResult = mono_runtime_invoke(getValueMethod, linesInstance, nullptr, nullptr);
    MonoString* resultStr = (MonoString*)getValueResult;
    std::cout << "Source text: " << mono_string_to_utf8(resultStr) << std::endl;

    // Виклик GetLength()
    MonoMethodDesc* getLengthDesc = mono_method_desc_new("Lines:GetLength()", NULL);
    MonoMethod* getLengthMethod = mono_method_desc_search_in_image(getLengthDesc, image);
    MonoObject* lengthResult = mono_runtime_invoke(getLengthMethod, linesInstance, nullptr, nullptr);
    int length = *(int*)mono_object_unbox(lengthResult);
    std::cout << "Lines length: " << length << std::endl;

    // Виклик SortAscending()
    MonoMethodDesc* sortDesc = mono_method_desc_new("Lines:SortAscending()", NULL);
    MonoMethod* sortMethod = mono_method_desc_search_in_image(sortDesc, image);
    MonoObject* sortedResult = mono_runtime_invoke(sortMethod, linesInstance, nullptr, nullptr);
    MonoString* sortedStr = (MonoString*)sortedResult;
    std::cout << "Sorted line: " << mono_string_to_utf8(sortedStr) << std::endl;

    mono_jit_cleanup(domain);
    return 0;
}