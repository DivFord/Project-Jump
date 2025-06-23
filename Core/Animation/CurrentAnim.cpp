#include "CurrentAnim.h"

#include <animControl.h>

#include <iostream>

CurrentAnim::CurrentAnim(AnimControl* anim)
{
	this->anim = anim;
}

void CurrentAnim::play_anim(bool looping)
{
	if (anim->is_playing())
		return;
	if (looping)
		anim->loop(true);
	else
		anim->play();
}

void CurrentAnim::stop_anim()
{
	anim->stop();
}

void CurrentAnim::set_anim_speed(float speed)
{
	anim->set_play_rate(speed);
}

void CurrentAnim::set_anim_relative_time(float relativeTime)
{
	double frame = relativeTime * anim->get_num_frames();
	std::cout << frame << '\n';
	anim->pose(frame);
}