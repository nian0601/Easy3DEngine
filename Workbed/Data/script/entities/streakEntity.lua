local startPosX = 0
local startPosY = 0
local startPosZ = 10
local radius = 25

local timer = 0
calcMovement = function(aEntity, aElapsed)
	timer = timer + aElapsed

	if(timer > 3.14 * 2) then
		timer = 0
	end

	local posX = startPosX + (math.cos(timer) * radius)
	local posY = startPosY + (math.sin(timer) * 10)

	aEntity:SetPosition(posX, posY, startPosZ)
end

entity = {
	graphicsComponent = {
		file = "Data/resources/model/companion/companion.fbx",
		model = {
			shader = "Data/effect/3D/BasicEffect.fx"
		}
	},
	physicsComponent = {
		speed = 20,
	},
	scriptComponent = {
		onUpdate = function(aEntity, aElapsed)
			calcMovement(aEntity, aElapsed)
		end,
	},
	emitterComponent = {
		file = "Data/script/emitters/streak.xml"
	}
}