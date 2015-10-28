inputMessage = function(aEntity, aX, aY, aZ)
	moveMsg = {
		type = "move",
		x = aX,
		y = aY,
		z = aZ
	}
	aEntity:SendLuaMessage(moveMsg)
end

colorMessage = function(aEntity, aR, aG, aB, aA)
	msg = {
		type = "color",
		r = aR,
		g = aG,
		b = aB,
		a = aA
	}
	aEntity:SendLuaMessage(msg)
end

local timer = 0
local direction = 0
calcMovement = function(aEntity, aElapsed)
	timer = timer + aElapsed

	if(timer > 1) then
		timer = 0
		direction = direction + 1
		if(direction >= 4) then
			direction = 0
		end
	end

	if(direction == 0) then
		inputMessage(aEntity, 0, -1, 0)
	elseif(direction == 1) then
		inputMessage(aEntity, 1, 0, 0)
	elseif(direction == 2) then
		inputMessage(aEntity, 0, 1, 0)
	elseif(direction == 3) then
		inputMessage(aEntity, -1, 0, 0)
	end

end

entity = {
	graphicsComponent = {
		file = "Data/resources/model/companion/companion.fbx",
		model = {
			shader = "Data/effect/3D/BasicEffect.fx"
		}
	},
	physicsComponent = {
		speed = 100,
	},
	scriptComponent = {
		onUpdate = function(aEntity, aElapsed)
			health = aEntity:GetHealth()
			if(health > 80) then
				colorMessage(aEntity, 1, 1, 1, 1)
			elseif(health > 50) then
				colorMessage(aEntity, 1, 0.5, 0.5, 1)
			elseif(health > 20) then
				colorMessage(aEntity, 1, 0.3, 0.3, 1)
			else
				colorMessage(aEntity, 0, 0, 0, 1)
			end
		end,
	},
	collisionComponent = {
		radius = 1,
		offset = {
			x = 0,
			y = 0,
			z = 0
		},
		onCollision = function(aEntity, aOtherEntity)
			colorMessage(aOtherEntity, 1, 0, 0, 1)
		end
	},
	healthComponent = {
		health = 100
	}
}