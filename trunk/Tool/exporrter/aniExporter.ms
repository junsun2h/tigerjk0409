struct ANI_TRANSFORM
(
	pos,
	rot
)

fn fnCollectObject arrNodes obj = 
(
	append arrNodes obj
	
	for c in obj.children do
		fnCollectObject arrNodes c
)

rollout treeview_rollout "Animation Exporter"
(
	dotNetControl tv "TreeView" width: 160 height:150 align:#center
	
	button BTN_insert "insert" width:80 height:24	
	button BTN_delete "delete" width:80 height:24
	button BTN_export "export Motion" width:80 height:24
	button BTN_exportActor "export Actor" width:80 height:24
	
	spinner SPIN_bakeInterval "Bake Interval" width:80 height:24 range:[1,100,5] type:#integer
	spinner SPIN_startFrame "Start Frame" width:80 height:24 type:#integer
	spinner SPIN_endFrame "End Framel" width:80 height:24 type:#integer
	checkbox CHL_loop "Loop" width:80 height:24 enabled:true checked:false

	on treeview_rollout open do
	(
		tv.indent = 11
		SPIN_startFrame.range = [0,animationRange.end,0]	
		SPIN_endFrame.range = [1,animationRange.end,1]
	)
		
	fn TV_addChildren parent child=
	(
		newNode = parent.Nodes.add child.name child.name 
		
		for c in child.children do 
			TV_addChildren newNode c
	)

	fn TV_Insert obj =
	(
		if tv.nodes.count > 0 then
			for i = 0 to tv.nodes.count - 1 do 
				if tv.nodes.item[i].text == obj.name then 
					return true
		
		TV_addChildren tv obj
 	)

	fn BakeAnimation arrNodes =
	(
		snap = #()			
		for o in arrNodes do
		(
			matLocal = copy o.transform
			if o.parent != undefined then 
				matLocal = o.transform * ( inverse o.parent.transform )
		
			append snap ( ANI_TRANSFORM matLocal.pos matLocal.rotation )
		)
		snap
	)
	
	on BTN_insert pressed  do
	(
		for o in  $selection do
		(
			if classof o == biped_object then
				TV_Insert o.controller.rootNode
			else
				TV_Insert o
		)
	)
	
	on BTN_delete pressed  do
	(
		if tv.selectedNode != undefined then
		(
			tv.nodes.remove tv.selectedNode
			tv.selectedNode = undefined
		)
	)


	on BTN_export pressed  do
	(
		file = getSaveFileName types:"animation file(*.motion)|*.motion"
		filePath = getFilenamePath file
		fileName = filenameFromPath file
		
		if tv.nodes.count == 0 then
			return false
		
		out_file = createfile file
		
		-- export animation info
		format "framerate %\n" frameRate to:out_file
		format "totalFrame %\n" ( SPIN_endFrame.value - SPIN_startFrame.value ) to:out_file
		format "bakeInterval %\n" SPIN_bakeInterval.value to:out_file
		
		for i = 0 to tv.nodes.count - 1 do
		(
			arrNodes = #()
			arrSnap = #()

			obj = getnodebyname tv.nodes.item[i].text
			if obj != undefined then
				fnCollectObject arrNodes obj
			
			animate on
			undo off
			
			lastSanpedFrame = 0
			for t = SPIN_startFrame.value to SPIN_endFrame.value by SPIN_bakeInterval.value do at time t 
			(
				append arrSnap ( BakeAnimation arrNodes ) 
				lastSanpedFrame = t	
			)
			
			-- bake last frame
			if lastSanpedFrame != SPIN_endFrame.value then at time SPIN_endFrame.value
			(
				append arrSnap ( BakeAnimation arrNodes )
			)
			
			for n = 1 to arrNodes.count do 
			(
				if arrNodes[n].parent != undefined then
					format "\nnode/%/%/\n" arrNodes[n].name arrNodes[n].parent.name to:out_file
				else
					format "\nnode/%/NONE/\n" arrNodes[n].name to:out_file		
				
				for s = 1 to arrSnap.count do
				(
					p = arrSnap[s][n].pos
					r = arrSnap[s][n].rot	
					format "%/%/%/%/%/%/%/\n" p.x p.y p.z r.x r.y r.z r.w to:out_file
				)
			)
		)

		close out_file
		
		tv.nodes.clear()
	)
	
	on BTN_exportActor pressed  do
	(
		file = getSaveFileName types:"animation file(*.actor)|*.actor"
		filePath = getFilenamePath file
		fileName = filenameFromPath file
		
		if tv.nodes.count == 0 then
			return false
		
		out_file = createfile file

		
		for i = 0 to tv.nodes.count - 1 do
		(
			arrNodes = #()
			arrSnap = #()

			obj = getnodebyname tv.nodes.item[i].text
			if obj != undefined then
				fnCollectObject arrNodes obj
	
			append arrSnap ( BakeAnimation arrNodes )
			
			for n = 1 to arrNodes.count do 
			(
				if arrNodes[n].parent != undefined then
					format "\nnode/%/%/\n" arrNodes[n].name arrNodes[n].parent.name to:out_file
				else
					format "\nnode/%/NONE/\n" arrNodes[n].name to:out_file		
				
				for s = 1 to arrSnap.count do
				(
					p = arrSnap[s][n].pos
					r = arrSnap[s][n].rot	
					format "%/%/%/%/%/%/%/\n" p.x p.y p.z r.x r.y r.z r.w to:out_file
				)
			)
		)

		close out_file
		
		tv.nodes.clear()
	)	
)
try(destroyDialog treeview_rollout)catch()
createDialog treeview_rollout 162 400

