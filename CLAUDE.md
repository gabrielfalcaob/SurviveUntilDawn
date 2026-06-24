\# Diretrizes do Projeto (SurviveUntilDawn)



\- \*\*Foco de Trabalho:\*\* Concentre suas buscas, leituras e modificações EXCLUSIVAMENTE nos arquivos de código-fonte (`.cpp` e `.h`).

- **Visual Studio Project Files:** You are ALLOWED to modify `.vcxproj` and `.vcxproj.filters` files, but ONLY for the purpose of adding, removing, or updating references to `.cpp` and `.h` files. Do not change build configurations, linkers, or compiler settings.
- **File Management:** Whenever you create, rename, or delete a `.cpp` or `.h` file, you MUST immediately parse the `.vcxproj` and `.vcxproj.filters` XML files and update the `<ItemGroup>` elements (like `<ClCompile Include="..." />` and `<ClInclude Include="..." />`) so the files appear in Visual Studio automatically.

\- \*\*Compilação:\*\* A compilação é feita manualmente pelo usuário via Visual Studio. Não crie arquivos de build (como CMakeLists.txt ou Makefiles) e não tente compilar o código pelo terminal.

