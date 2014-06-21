namespace cppcraft
{
	class Sector;
	
	class PrecompSchedule
	{
	public:
		const Sector* sector;
		
		PrecompSchedule(const Sector& s):
			sector(&s) {}
		
		bool schedule();
	};
	
	class PrecompScheduler
	{
	public:
		static void add(const Sector& sector);
		static void scheduling();
		// used by teleport
		static void reset();
	};
	
}
