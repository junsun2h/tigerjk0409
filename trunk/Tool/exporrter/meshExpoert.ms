struct BONE_WEIGHT
(
	ID,
	weight
)

fn getMapName map = 
(
	if map == undefined then
		return undefined
	else if classof map == Bitmaptexture then
		return filenameFromPath map.filename
	else if classof map == Normal_Bump then
		return filenameFromPath map.normal_map.filename
	else 
		return undefined
)

fn fnWriteStandardMtrl mtrl filePath = 
(
	out_name = filePath + "/" + mtrl.name + ".xml"
	out_file = createfile out_name
	
	format "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" to:out_file
	format "<mtrl shader=\"%\" twoSided=\"%\" opacity=\"%\" specularLevel=\"%\" glossiness=\"%\">\n" mtrl.shaderByName mtrl.twoSided mtrl.opacity mtrl.specularLevel mtrl.glossiness to:out_file

	format "<map>\n" to:out_file

	if mtrl.shaderType  == 1 or mtrl.shaderType == 2 or  mtrl.shaderType == 5 do -- Blinn, Metal, Phong shader
	(
		channels = #("AmbientMap", "DiffuseMap", "SpecularMap", "SpecularLevelMap", "GlossinessMap", "SelfIllumMap", 
							"OpacityMap", "FilterMap", "BumpMap", "ReflectionMap", "RefractionMap", "DisplacementMap" )

		for i = 1 to channels.count do
		(
			if  mtrl.maps[i] == undefined or getMapName mtrl.maps[i] == undefined then 
				continue
				
			format "<% texture=\"%\"/>\n" channels[i] (getMapName mtrl.maps[i]) to:out_file
		)
	)
	
	format "</map>\n" to:out_file
	format "</mtrl>" to:out_file
	
	close out_file
)

fn fnExportMtrl tmesh filePath = 
(		
	if classof tmesh.material == Standardmaterial then
	(
		fnWriteStandardMtrl tmesh.material filePath
	)
	else if classof tmesh.material == Multimaterial then
	(
		for mtrl in tmesh.material do
		(
			if classof mtrl == Standardmaterial then fnWriteStandardMtrl mtrl filePath		
		)		
	)
)


fn fnExportMesh tmesh filePath =
(
	arrVert = #()
	arrNormal = #()
	arrUV = #()	
	
	arrFaces = #()
	arrUVFaces = #()
	arrNormalFaces = #()	
	arrMultiMtrlIDFaces = #()
	
	arrBoneNames = #()
	arrBonePos = #()
	arrBoneRot = #()	
	
	arrVertWeight = #()	

	max modify mode
	
	addModifier tmesh ( Edit_Mesh() )

	out_name = filePath + "/" + tmesh.name + ".mesh"
	out_file = createfile out_name
	
	-- export transform
	pos = in coordsys parent tmesh.pos;
	rot = in coordsys parent tmesh.rotation;	
	format "transform\n% % %\n% % % %\n" pos.x pos.y pos.z rot.x rot.y rot.z rot.z to:out_file
	
	-- export material
	format "\nmaterial\n%\n" ( classOf tmesh.material ) to:out_file
	if classOf tmesh.material == Multimaterial then
	(
		format "%\n" tmesh.material.count to:out_file
		for i =1 to tmesh.material.count do 
		(
			if tmesh.material[i] == undefined then 
				format "%\n" none to:out_file
			else 
				format "%\n" tmesh.material[i] .name to:out_file	
		)
	)
	else if classOf tmesh.material != UndefinedClass then 
		format "%\n" tmesh.material.name to:out_file

	addModifier tmesh ( Edit_Normals() ) -- to get normal information	

	-- export vertex information
	for f = 1 to tmesh.numverts do append arrVert ( in coordsys local meshOp.getVert tmesh f )
	for f = 1 to tmesh.numTVerts do append arrUV ( getTVert tmesh f )	
	for f = 1 to tmesh.Edit_Normals.GetNumNormals() do append arrNormal ( tmesh.Edit_Normals.GetNormal f )
	
	-- export faces information	
	for f = 1 to tmesh.numfaces do
	(
		normalFaces = #()
		for j=1 to 3 do append normalFaces ( tmesh.Edit_Normals.GetNormalID f j )
	
		append arrNormalFaces normalFaces
		append arrFaces ( getFace tmesh f )	
		append arrUVFaces ( getTVFace tmesh f )
		append arrMultiMtrlIDFaces ( getfacematid tmesh f )	
	)
	
	deleteModifier tmesh tmesh.modifiers[#Edit_Normals]
	deleteModifier tmesh tmesh.modifiers[#Edit_Mesh]
	
	-- export weight
	local modSkin= tmesh.modifiers[#Skin]
	if modSkin != undefined and (SkinOps.GetNumberBones modSkin) > 0 then 
	(	
		-- export bone names
		boneCount = SkinOps.GetNumberBones modSkin	
		for i = 1 to boneCount do
		(
			boneName =  SkinOps.GetBoneName modSkin i 1
			obj	= getnodebyname boneName
			
			matLocal = copy obj.transform

			if obj.parent == undefined then 
			(
				append arrBoneNames ( boneName + " NULL" )
			)
			else
			(
				matLocal = obj.transform * ( inverse obj.parent.transform )
				append arrBoneNames ( boneName + " " + obj.parent .name )
			)
			
			append arrBonePos matLocal.pos
			append arrBoneRot matLocal.rotation
		)
		
		-- export viertex weight
		modPanel.setCurrentObject modSkin
		for i = 1 to  tmesh.numverts do
		(
			boneCount = SkinOps.GetVertexWeightCount modSkin i
			local bwSet = #()
			for j = 1 to boneCount do
			(
				append bwSet ( BONE_WEIGHT ( SkinOps.GetVertexWeightBoneID modSkin i j ) ( SkinOps.GetVertexWeight modSkin i j ) )
			)		
			append arrVertWeight bwSet	
		)	
	)
	
	format "\nposition %\n" arrVert.count to:out_file
	for vert in arrVert do format "% % %\n" vert.x vert.y vert.z to:out_file
	format "end\n" to:out_file

	format "\nnormal %\n" arrNormal.count to:out_file
	for normal in arrNormal do format "% % %\n" normal.x normal.y normal.z to:out_file
	format "end\n" to:out_file

	format "\nuv %\n" arrUV.count to:out_file
	for uv in arrUV do format "% %\n" uv.x uv.y to:out_file
	format "end\n" to:out_file

	format "\nfacePos %\n" arrFaces.count to:out_file
	for face in arrFaces do format "% % %\n" face[1] face[2] face[3] to:out_file
	format "end\n" to:out_file

	format "\nfaceUV %\n" arrUVFaces.count to:out_file
	for uvFace in arrUVFaces do format "% % %\n" uvFace[1] uvFace[2] uvFace[3] to:out_file
	format "end\n" to:out_file

	format "\nfaceNormal %\n" arrNormalFaces.count to:out_file
	for normalFace in arrNormalFaces do format "% % %\n" normalFace[1] normalFace[2] normalFace[3] to:out_file
	format "end\n" to:out_file
	
	if classOf tmesh.material == Multimaterial then
	(
		format "\nfaceID %\n" arrMultiMtrlIDFaces.count to:out_file
		for faceID in arrMultiMtrlIDFaces do format "%\n" faceID to:out_file
		format "end\n" to:out_file	
	)		

	if arrBoneNames.count > 0 then
	(
		format "\nweight %\n" arrVertWeight.count to:out_file		
		for vertWeight in arrVertWeight  do
		(
			format "% " vertWeight.count to:out_file
			for w in vertWeight  do format "% % " w.ID w.weight to:out_file
			format "\n" to:out_file			
		)
		format "end\n" to:out_file
	)		
		
	close out_file
	
	-- export material files
	fnExportMtrl tmesh filePath
	
	-- export actor file
	if arrBoneNames.count > 0 then
	(
		out_name = filePath + "/" + tmesh.name + ".tac"
		out_file = createfile out_name
		
		format "bone %\n" arrBoneNames.count to:out_file

		for i = 1 to arrBoneNames.count do 
		(
			p = arrBonePos[i]
			r = arrBoneRot[i]				
			format "% % % % % % % %\n" arrBoneNames[i] p.x p.y p.z r.x r.y r.z r.w to:out_file
		)
		
		format "end\n" to:out_file
		
		close out_file	
	)		
)

rollout RolloutExporter "MeshExporter" width:162 height:300
(
	multilistBox LISTBOX_Nodes "Meshes" pos:[8,16] width:144 height:7
	button BTN_export "export" pos:[49,177] width:64 height:24
	button BTN_Insert "insert" pos:[9,141] width:64 height:24
	button BTN_Delete "delete" pos:[86,141] width:64 height:24
	
	on BTN_export pressed  do
	(
		file = getSaveFileName types:"animation file(*.txt)|*.txt"
		filePath = getFilenamePath file
		for o in LISTBOX_Nodes.items do fnExportMesh ( getNodeByName o exact:true ) filePath
	)

	on BTN_Insert pressed  do
	(
		local listObjs = LISTBOX_Nodes.items
		for o in  $selection do
		(
			if classof o == Editable_mesh or classof o == PolyMeshObject do appendifunique listObjs o.name
		)
		
		LISTBOX_Nodes.items = listObjs	
	)
	
	on BTN_Delete pressed  do
	(
		for i in LISTBOX_Nodes.selection do
		(
			deleteItem LISTBOX_Nodes.items (findItem LISTBOX_Nodes.items LISTBOX_Nodes.items[i] )
		)
		LISTBOX_Nodes.items = LISTBOX_Nodes.items		
	)
)

createDialog RolloutExporter