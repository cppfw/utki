msbuild msvs_solution/msvs_solution.sln /target:Clean /property:Configuration=Debug /property:Platform=x86
msbuild msvs_solution/msvs_solution.sln /target:Clean /property:Configuration=Debug /property:Platform=x64
msbuild msvs_solution/msvs_solution.sln /target:Clean /property:Configuration=Release /property:Platform=x86
msbuild msvs_solution/msvs_solution.sln /target:Clean /property:Configuration=Release /property:Platform=x64

msbuild msvs_solution/msvs_solution.sln /property:Configuration=Debug /property:Platform=x86
msbuild msvs_solution/msvs_solution.sln /property:Configuration=Debug /property:Platform=x64
msbuild msvs_solution/msvs_solution.sln /property:Configuration=Release /property:Platform=x86
msbuild msvs_solution/msvs_solution.sln /property:Configuration=Release /property:Platform=x64

powershell Write-NuGetPackage nuget.autopkg
