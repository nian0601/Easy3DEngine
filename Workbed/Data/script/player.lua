inputMessage = function(aEntity, aX, aY, aZ)
	moveMsg = {
		type = "move",
		x = aX,
		y = aY,
		z = aZ
	}
	aEntity:SendLuaMessage(moveMsg)
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

entity = {
	graphicsComponent = {
		file = "Data/resources/model/PBL_metalness_arm/pbl_metalness_arm_binary.fbx",
		model = {
			shader = "Data/effect/3D/BasicEffect.fx"
		}
	},
	--[[inputComponent = {
		onWDown = function(aEntity)
			inputMessage(aEntity, 0, 0, 1)
		end,
		onSDown = function(aEntity)
			inputMessage(aEntity, 0, 0, -1)
		end,
		onADown = function(aEntity)
			inputMessage(aEntity, -1, 0, 0)
		end,
		onDDown = function(aEntity)
			inputMessage(aEntity, 1, 0, 0)
		end
	},
	physicsComponent = {
		speed = 20,
	},
	scriptComponent = {
		onUpdate = function(aEntity, aElapsed)
			colorMessage(aEntity, 1, 1, 1, 1);
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
			--aOtherEntity:TakeDamage(10)
			aEntity:SetPosition(0, 0, 10)
			aEntity:StopMovement()
			colorMessage(aEntity, 0, 1, 0, 1)
		end
	},
	healthComponent = {
		health = 100
	}]]--
}