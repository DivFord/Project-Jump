#pragma once

class AnimControl;

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

