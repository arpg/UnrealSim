using System.IO;
using UnrealBuildTool;

public class UnrealSim : ModuleRules
{
	public UnrealSim(TargetInfo Target)
	{
    AddCoreDependencies();
    AddExternalDependencies();
	}

  private void AddCoreDependencies()
  {
    string[] publicRange = { "Core", "CoreUObject", "Engine", "InputCore" };
		PublicDependencyModuleNames.AddRange(publicRange);

    string[] privateRange = { };
		PrivateDependencyModuleNames.AddRange(privateRange);
  }

  private void AddExternalDependencies()
  {
    // append to include paths
    PublicIncludePaths.Add("/usr/local/include/");

    // append to libraries
    PublicAdditionalLibraries.Add("/usr/local/lib/libzmq.a");
    PublicAdditionalLibraries.Add("/usr/local/lib/libzmqpp-static.a");
    PublicAdditionalLibraries.Add("/usr/local/lib/libprotobuf.a");
    PublicAdditionalLibraries.Add("/usr/local/lib/libnode.a");
    PublicAdditionalLibraries.Add("/usr/local/Cellar/glog/0.3.4/lib/libglog.a");
    PublicAdditionalLibraries.Add("/usr/local/Cellar/gflags/2.1.2/lib/libgflags.a");

    // unreal doesn't build with runtime type info (RTTI)
    Definitions.Add("GOOGLE_PROTOBUF_NO_RTTI=1");
  }
}
