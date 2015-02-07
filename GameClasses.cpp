#include "GameClasses.h"

// Tool Belt

ToolBelt::ToolBelt( int amountOfBuckets, BitmapFont* font, Texture* blankBucket, Texture* selectedBucket )
{
	if( amountOfBuckets > 10 )
		amountOfBuckets = 10;

	if( amountOfBuckets < 0 )
		amountOfBuckets = 1;

	this->amountOfBuckets = amountOfBuckets;

	buckets = new StaticObject2D[amountOfBuckets];
	lables = new std::string[amountOfBuckets];
	double maxWidth = ( amountOfBuckets * ( blankBucket->textureSize.w + 5 ) );
	std::stringstream ss;

	for( int i = 0; i < amountOfBuckets; i++ )
	{
		buckets[i].SetTexture( blankBucket->textureID );
		buckets[i].setSize( blankBucket->textureSize.w, blankBucket->textureSize.h, 0.0 );
		buckets[i].setPosition( ( blankBucket->textureSize.w + 5 ) * i - ( maxWidth / 2.0 ) + blankBucket->textureSize.w / 2.0, 0.0, 0.0 );

		ss.str("");

		if( i == 9 )
			ss << "0 -" << "???";
		else
			ss << i+1 << "-" << "???";

		lables[i] = ss.str();
	}

	selectedTool = 0;
	this->selectedBucket.SetTexture( selectedBucket->textureID );
	this->selectedBucket.setSize( selectedBucket->textureSize.w, selectedBucket->textureSize.h, 0.0 );
	bucketLable.SetFont( font );
}

void ToolBelt::SetPosition( float x, float y )
{
	double maxWidth = ( amountOfBuckets * ( buckets[0].GetSize().w + 5 ) );
	for( int i = 0; i < amountOfBuckets; i++ )
		buckets[i].setPosition( ( buckets[i].GetSize().w + 5 ) * i + x - ( maxWidth / 2.0 ) + buckets[i].GetSize().w / 2.0, y, 0.0 );
}

void ToolBelt::SetBucketLable( int bucket, std::string lable )
{
	bucket--;

	if( bucket < 0 || bucket > amountOfBuckets )
		return;

	std::stringstream ss;
	ss << bucket+1 << "-" << lable;

	lables[bucket] = ss.str();
}

void ToolBelt::SetSelectedItem( int tool )
{
	tool--;
	selectedTool = tool;
}

int ToolBelt::GetSelectedTool()
{
	return selectedTool;
}

void ToolBelt::draw()
{
	if( !visible )
		return;

	for( int i = 0; i < amountOfBuckets; i++ )
	{
		buckets[i].draw();
		bucketLable.SetText( lables[i] );
		bucketLable.SetPosition( static_cast<float>( buckets[i].GetPosition().x ), static_cast<float>( buckets[i].GetPosition().y ) );
		bucketLable.draw();

		if( i == selectedTool )
		{
			selectedBucket.setPosition( buckets[i].GetPosition().x, buckets[i].GetPosition().y, 0.0 );
			selectedBucket.draw();
		}
	}

}

void ToolBelt::handleEvent( SDL_Event& e )
{
	if( e.type == SDL_KEYDOWN )
	{
		for( int i = 49; i < 49+amountOfBuckets; i++ )
		{
			if( e.key.keysym.sym == i )
			{
				selectedTool = i - 49;
				return;
			}

			if( e.key.keysym.sym == 48 && amountOfBuckets == 10 )
			{
				selectedTool = 9;
				return;
			}
		}
	}
}

//
// Player Class
//

Player::Player()
{
	moveSpeed = 10.0;
	jumpHeight = 10.0;

	playerSize.h = 2.0;
	playerSize.w = 1.0;
	playerSize.d = 1.0;

	position.y = 0.5;

	flying = false;
	disableMouseMovement = false;
	hasMoved = false;

	moveTimer.start();

}

void Player::move()
{
	const double MAX_STEP = 1.0;

	if( flying && position.y > 0 )
	{

		position.x += -(sin( Math::DegToRad( rotation.y ) ) * velocity.z * ( position.y / 5.0 ) ) * Math::timeFactor( moveTimer.get_ticks() );
		position.z += (cos( Math::DegToRad( rotation.y ) ) * velocity.z * ( position.y / 5.0 ) ) * Math::timeFactor( moveTimer.get_ticks() );
		position.x += (cos( Math::DegToRad( rotation.y ) ) * velocity.x * ( position.y / 5.0 ) ) * Math::timeFactor( moveTimer.get_ticks() );
		position.z += (sin( Math::DegToRad( rotation.y ) ) * velocity.x * ( position.y / 5.0 ) ) * Math::timeFactor( moveTimer.get_ticks() );
	}
	else
	{

		position.x += -(sin( Math::DegToRad( rotation.y ) ) * velocity.z ) * Math::timeFactor( moveTimer.get_ticks() );
		position.z += (cos( Math::DegToRad( rotation.y ) ) * velocity.z ) * Math::timeFactor( moveTimer.get_ticks() );
		position.x += (cos( Math::DegToRad( rotation.y ) ) * velocity.x ) * Math::timeFactor( moveTimer.get_ticks() );
		position.z += (sin( Math::DegToRad( rotation.y ) ) * velocity.x ) * Math::timeFactor( moveTimer.get_ticks() );
	}

	if( hasMoved && moveTimer.is_started())
	{
		if( !flying ) // y movement
		{
			double bottom = position.y - playerSize.h / 2.0;

			if( bottom > floor )
			{
				if( velocity.y == 0.0 && abs( bottom - floor ) < MAX_STEP )
				{
					position.y = floor + playerSize.h / 2.0;
				}
				else
				{
					velocity.y -= jumpHeight * Math::timeFactor( moveTimer.get_ticks() );
				}
			}
			else
			{
				if( velocity.y <= 0.0 )
				{
					position.y = floor + playerSize.h / 2.0;
					velocity.y = 0.0;
				}
			}

			if( velocity.y != 0.0 )
			{
				position.y += velocity.y * Math::timeFactor( moveTimer.get_ticks() );
			}
		}

		moveTimer.start();
	}

	if( !hasMoved )
	{
		moveTimer.start();
	}
}

void Player::Pause()
{
	moveTimer.stop();
}
void Player::Unpause()
{
	moveTimer.start();
}

void Player::handleEvent( SDL_Event& e )
{
	if( e.type == SDL_KEYDOWN )
	{
		switch( e.key.keysym.sym )
		{
			case SDLK_w:
			case SDLK_UP:
				velocity.z = -moveSpeed;
				hasMoved = true;
			break;

			case SDLK_a:
			case SDLK_LEFT:
				velocity.x = -moveSpeed;
				hasMoved = true;
			break;

			case SDLK_s:
			case SDLK_DOWN:
				velocity.z = moveSpeed;
				hasMoved = true;
			break;

			case SDLK_d:
			case SDLK_RIGHT:
				velocity.x = moveSpeed;
				hasMoved = true;
			break;

			case SDLK_SPACE:
				if( velocity.y == 0.0 && moveTimer.is_started() )
					velocity.y = jumpHeight;
			break;

			case SDLK_e:
				if( moveTimer.is_started() )
					flying = !flying;
			break;

			case SDLK_q:
				if( flying && moveTimer.is_started() )
					position.y += 6.0;
			break;

			case SDLK_z:
				if( flying && moveTimer.is_started() )
					position.y -= 6.0;
			break;

			case SDLK_ESCAPE:
				disableMouseMovement = !disableMouseMovement;
			break;
		}
	}

	if( e.type == SDL_KEYUP )
	{
		switch( e.key.keysym.sym )
		{
			case SDLK_w:
			case SDLK_UP:
				if( velocity.z < 0 )
					velocity.z = 0;
			break;

			case SDLK_a:
			case SDLK_LEFT:
				if( velocity.x < 0 )
					velocity.x = 0;
			break;

			case SDLK_s:
			case SDLK_DOWN:
				if( velocity.z > 0 )
					velocity.z = 0;
			break;

			case SDLK_d:
			case SDLK_RIGHT:
				if( velocity.x > 0 )
					velocity.x = 0;
			break;
		}
	}

	// If the mouse moves change the two view angles
	if( e.type == SDL_MOUSEMOTION && !disableMouseMovement )
	{
		rotation.x -= e.motion.yrel / 200.0;
		rotation.y += e.motion.xrel / 2.0;
	}

	if( e.type == SDL_MOUSEBUTTONDOWN && flying && !disableMouseMovement )
	{

		if( e.button.button == SDL_BUTTON_WHEELDOWN )
		{
			position.y += 6.0;
		}

		if( e.button.button == SDL_BUTTON_WHEELUP )
		{
			position.y -= 6.0;
		}

	}

}

void Player::SetPosition( Point3F position )
{
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
}

Point3D Player::GetPosition()
{
	return position;
}

Rotation3D Player::GetRotation()
{
	return rotation;
}

void Player::SetFloor( double y )
{
	this->floor = y;

}

bool Player::isFlying()
{
	return flying;
}

bool Player::hasDisabledMouseMovement()
{
	return disableMouseMovement;
}
