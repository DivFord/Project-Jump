#pragma once

class AnimControl;

/// <summary>
/// An interface into Panda's AnimControl class, so that AnimGraph can
/// set some values without needing to be directly coupled.
/// </summary>
class CurrentAnim
{
	AnimControl* anim;

public:
	CurrentAnim(AnimControl* anim);

	void play_anim(bool looping);
	void stop_anim();
	void set_anim_speed(float speed);
	void set_anim_relative_time(float relativeTime);
};

